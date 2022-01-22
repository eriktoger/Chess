import { get } from 'svelte/store';
import type {
	BoardPosition,
	SquareArray,
	Images,
	RowArray,
	GamePtr,
	Move,
	Piece
} from '../types/chess';
import { getImage } from '../functions/helpers';
import {
	showPromotionModal,
	modalPlacement,
	promotionColor,
	onChoosePromotion,
	gameStatus
} from '../stores/modals';
import { setPlayerPerspective, createNewGame, usingTouch } from '../stores/game';

class Board {
	canvas: HTMLCanvasElement;
	context: CanvasRenderingContext2D;
	squareSize: number;
	gamePtr: GamePtr;
	mouseDown = false;
	currentSquare: BoardPosition;
	possibleMoves: SquareArray;
	selectedSquare: BoardPosition;
	images: Images;
	squares: RowArray;
	playerPerspective: string;
	lastMove: Move;
	usingTouch: boolean;

	constructor(gamePtr: GamePtr, images: Images) {
		this.gamePtr = gamePtr;
		this.squares = this.gamePtr.getSquares();
		this.images = images;
		this.playerPerspective = 'White';

		this.canvas = <HTMLCanvasElement>document.getElementById('canvas');

		this.context = this.canvas.getContext('2d');

		this.resizeBoard();

		setPlayerPerspective.set((perspective) => {
			this.playerPerspective = perspective;
		});

		createNewGame.set((timePerMove: number, useOpeningBook: boolean) => {
			this.gamePtr.newGame(this.playerPerspective, timePerMove, useOpeningBook);
			if (this.playerPerspective === 'Black') {
				this.makeComputerMove();
			}
			this.squares = this.gamePtr.getSquares();
			this.lastMove = { startRow: -1, startCol: -1, endRow: -1, endCol: -1 };
			this.drawBoard();
		});

		usingTouch.subscribe((value) => {
			this.usingTouch = value;
		});

		this.canvas.addEventListener('mousedown', ({ clientX, clientY }) => {
			!this.usingTouch && this.onMouseDown(clientX, clientY);
		});

		this.canvas.addEventListener('mousemove', ({ clientX, clientY }) => {
			!this.usingTouch && this.onMouseMove(clientX, clientY);
		});

		this.canvas.addEventListener('mouseup', ({ clientX, clientY }) => {
			!this.usingTouch && this.onMouseUp(clientX, clientY);
		});

		this.canvas.addEventListener('touchstart', (event) => {
			const { clientX, clientY } = event.touches?.[0];
			this.usingTouch && this.onTouch(clientX, clientY);
		});
	}

	resizeBoard = (): void => {
		this.squareSize = Math.min(window.innerHeight * 0.8, window.innerWidth * 0.8) / 8;
		this.canvas.height = this.squareSize * 8;
		this.canvas.width = this.squareSize * 8;

		const { left, top, width } = this.canvas.getBoundingClientRect();
		modalPlacement.set({ left, top, width });

		this.drawBoard();
	};

	drawPiece = (row: number, col: number, piece: Piece): void => {
		const { type, color } = piece;
		const { row: flippedRow, col: flippedCol } = this.flipPosition(
			this.currentSquare?.row,
			this.currentSquare?.col
		);
		if (row === flippedRow && col == flippedCol) {
			this.context.globalAlpha = 0.4;
		} else {
			this.context.globalAlpha = 1;
		}

		const image = getImage(type, color, this.images);
		image && this.drawImage(image, row, col);
		this.context.globalAlpha = 1;
	};

	drawImage = (image: HTMLImageElement, row: number, col: number): void => {
		this.context.drawImage(
			image,
			this.squareSize * col + this.squareSize / 40,
			this.squareSize * row + this.squareSize / 20,
			this.squareSize - this.squareSize / 20,
			this.squareSize - this.squareSize / 20
		);
	};

	drawBoard = (): void => {
		const { startRow = -1, startCol = -1, endRow = -1, endCol = -1 } = this.lastMove ?? {};
		for (let r = 0; r < this.squares.size(); r++) {
			for (let c = 0; c < this.squares.get(r).size(); c++) {
				const { piece, row, col, color } = this.squares.get(r).get(c);
				const { row: flippedRow, col: flippedCol } = this.flipPosition(row, col);
				this.drawSquare(flippedRow, flippedCol, color);

				if ((row === startRow && col == startCol) || (row === endRow && col == endCol)) {
					this.drawLastMove(flippedRow, flippedCol);
				}
				this.drawPiece(flippedRow, flippedCol, piece);
			}
		}
	};

	drawSquare = (row: number, col: number, color: string): void => {
		this.context.fillStyle = color === 'White' ? 'white' : 'grey';
		this.context.fillRect(
			col * this.squareSize,
			row * this.squareSize,
			this.squareSize,
			this.squareSize
		);
	};

	drawLastMove = (row: number, col: number): void => {
		this.context.fillStyle = 'green';
		this.context.globalAlpha = 0.2;
		this.context.fillRect(
			col * this.squareSize,
			row * this.squareSize,
			this.squareSize,
			this.squareSize
		);
		this.context.globalAlpha = 1;
	};

	getPossibleMoves = (clientX: number, clientY: number): void => {
		const [flippedRow, flippedCol] = this.calcRowColClicked(clientX, clientY);
		this.possibleMoves = this.gamePtr.calcAndGetLegalMoves(flippedRow, flippedCol);
		this.currentSquare =
			this.possibleMoves.size() > 0 ? { row: flippedRow, col: flippedCol } : { row: -1, col: -1 };
	};

	drawPossibleMoves = (type: string, color: string): void => {
		const image = getImage(type, color, this.images);
		for (let i = 0; i < this.possibleMoves.size(); i++) {
			const { row, col } = this.possibleMoves.get(i);
			const square = this.squares.get(row).get(col);
			const { row: flippedRow, col: flippedCol } = this.flipPosition(row, col);
			if (square.piece.type !== '') {
				this.drawCaptureSquare(flippedRow, flippedCol);
			} else {
				this.drawPossibleMove(flippedRow, flippedCol, image);
			}
		}
	};

	drawPossibleMove = (row: number, col: number, image: HTMLImageElement): void => {
		this.context.globalAlpha = 0.2;
		image && this.drawImage(image, row, col);
		this.context.globalAlpha = 1;
	};

	drawCaptureSquare = (row: number, col: number): void => {
		this.context.fillStyle = 'red';
		this.context.globalAlpha = 0.25;
		this.context.fillRect(
			col * this.squareSize,
			row * this.squareSize,
			this.squareSize,
			this.squareSize
		);
	};

	drawLiftedPiece = (clientX: number, clientY: number, image: HTMLImageElement): void => {
		const { left, top } = this.canvas.getBoundingClientRect();
		const centerToMouse = this.squareSize / 2;
		const row = (clientY - top - centerToMouse) / this.squareSize;
		const col = (clientX - left - centerToMouse) / this.squareSize;
		this.context.globalAlpha = 0.4;
		this.drawImage(image, row, col);
		this.context.globalAlpha = 1;
	};

	highlightSquare = (): void => {
		const { row: flippedRow, col: flippedCol } = this.flipPosition(
			this.selectedSquare?.row,
			this.selectedSquare?.col
		);
		this.context.beginPath();
		this.context.lineWidth = 4;
		this.context.rect(
			flippedCol * this.squareSize,
			flippedRow * this.squareSize,
			this.squareSize,
			this.squareSize
		);
		this.context.stroke();
	};

	getSelectedSquare = (clientX: number, clientY: number): void => {
		const [flippedRow, flippedCol] = this.calcRowColClicked(clientX, clientY);
		for (let i = 0; i < this.possibleMoves?.size(); i++) {
			const { row, col } = this.possibleMoves.get(i);
			if (flippedRow === row && flippedCol === col) {
				this.selectedSquare = { row, col };
				return;
			}
		}
		this.selectedSquare = null;
	};

	onMouseDown = (clientX: number, clientY: number): void => {
		this.getPossibleMoves(clientX, clientY);
		if (this.possibleMoves.size() > 0) {
			const {
				piece: { type, color }
			} = this.squares.get(this.currentSquare.row).get(this.currentSquare.col);
			this.drawPossibleMoves(type, color);

			this.mouseDown = true;
		}
	};

	onMouseMove = (clientX: number, clientY: number): void => {
		if (this.mouseDown) {
			this.getSelectedSquare(clientX, clientY);

			this.drawBoard();

			if (this.selectedSquare) {
				this.highlightSquare();
			}

			const {
				piece: { type, color }
			} = this.squares.get(this.currentSquare.row).get(this.currentSquare.col);

			this.drawPossibleMoves(type, color);

			const piece = getImage(type, color, this.images);

			this.drawLiftedPiece(clientX, clientY, piece);
		}
	};

	onMouseUp = (clientX: number, clientY: number): void => {
		if (this.mouseDown) {
			this.movePiece(clientX, clientY);
		}
	};

	movePiece = (clientX: number, clientY: number): void => {
		const [flippedRow, flippedCol] = this.calcRowColClicked(clientX, clientY);
		if (flippedRow === this.selectedSquare?.row && flippedCol == this.selectedSquare?.col) {
			this.doMovePiece();
		}
		this.currentSquare = null;
		this.mouseDown = false;
		this.drawBoard();
	};

	doMovePiece = (): void => {
		const isPawn =
			this.squares.get(this.currentSquare.row).get(this.currentSquare.col).piece.type === 'Pawn';
		const willPromote = this.selectedSquare.row == 0 || this.selectedSquare.row == 7;

		if (isPawn && willPromote) {
			this.onPromotion();
			return;
		}

		const { status, squares, lastMove } = this.gamePtr.makeAMove(
			this.currentSquare.row,
			this.currentSquare.col,
			this.selectedSquare.row,
			this.selectedSquare.col
		);
		this.lastMove = lastMove;
		this.squares = squares;
		gameStatus.set(status);
		this.makeComputerMove();
	};

	onTouch = (clientX: number, clientY: number): void => {
		this.getSelectedSquare(clientX, clientY);

		if (this.selectedSquare) {
			this.doMovePiece();
			this.possibleMoves = null;
		} else {
			this.onTouchPrepareMove(clientX, clientY);
		}
	};

	onTouchPrepareMove = (clientX: number, clientY: number): void => {
		this.getPossibleMoves(clientX, clientY);
		this.drawBoard();
		const [flippedRow, flippedCol] = this.calcRowColClicked(clientX, clientY);
		const {
			piece: { type, color }
		} = this.squares.get(flippedRow).get(flippedCol);
		this.drawPossibleMoves(type, color);
	};

	onPromotion = (): void => {
		promotionColor.set(this.gamePtr.getTurn());

		onChoosePromotion.set((type: string) => {
			this.gamePtr.setPromotionType(type);
			const { status, squares, lastMove } = this.gamePtr.makeAMove(
				this.currentSquare.row,
				this.currentSquare.col,
				this.selectedSquare.row,
				this.selectedSquare.col
			);
			this.lastMove = lastMove;
			this.squares = squares;
			this.currentSquare = null;
			this.mouseDown = false;
			this.drawBoard();
			this.makeComputerMove();
			gameStatus.set(status);
			showPromotionModal.set(false);
		});

		showPromotionModal.set(true);
	};

	makeComputerMove = (): void => {
		this.drawBoard();
		setTimeout(() => {
			if (get(gameStatus) === '') {
				const { status, squares, lastMove } = this.gamePtr.makeComputerMove();
				this.lastMove = lastMove;
				this.squares = squares;
				gameStatus.set(status);
				this.drawBoard();
			}
		}, 100);
	};

	flipPosition = (row: number, col: number): BoardPosition =>
		this.playerPerspective === 'White' ? { row, col } : { row: 7 - row, col: 7 - col };

	calcRowColClicked = (clientX: number, clientY: number): [number, number] => {
		const { left, top } = this.canvas.getBoundingClientRect();
		const row = Math.floor((clientY - top) / this.squareSize);
		const col = Math.floor((clientX - left) / this.squareSize);
		const { row: fRow, col: fCol } = this.flipPosition(row, col);

		return [fRow, fCol];
	};
}

export default Board;
