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
import { setPlayerPerspective, createNewGame } from '../stores/game';

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

		createNewGame.set((timePerMove: number) => {
			this.gamePtr.newGame(this.playerPerspective, timePerMove);
			if (this.playerPerspective === 'Black') {
				this.makeComputerMove();
			}
			this.squares = this.gamePtr.getSquares();
			this.lastMove = { startRow: -1, startCol: -1, endRow: -1, endCol: -1 };
			this.drawBoard();
		});

		this.canvas.addEventListener('mousedown', (event) => {
			this.onMouseDown(event);
		});

		this.canvas.addEventListener('mousemove', (event) => {
			this.onMouseMove(event);
		});

		this.canvas.addEventListener('mouseup', (event) => {
			this.onMouseUp(event);
		});
	}

	resizeBoard = (): void => {
		this.squareSize = Math.min(window.innerHeight - 300, window.innerWidth - 100) / 8;
		this.canvas.height = this.squareSize * 8;
		this.canvas.width = this.squareSize * 8;

		const { left, top, width } = this.canvas.getBoundingClientRect();
		modalPlacement.set({ left, top, width });

		this.drawBoard();
	};

	drawPiece = (row: number, col: number, piece: Piece): void => {
		const { type, color } = piece;
		if (row === this.currentSquare?.row && col == this.currentSquare?.col) {
			this.context.globalAlpha = 0.4;
		} else {
			this.context.globalAlpha = 1;
		}

		const image = getImage(type, color, this.images);
		image && this.drawImage(image, row, col);
		this.context.globalAlpha = 1;
	};

	drawImage = (image: HTMLImageElement, row: number, col: number): void => {
		//the offset needs to scale with the board.
		this.context.drawImage(
			image,
			this.squareSize * col + 2.5,
			this.squareSize * row + 5,
			this.squareSize - 5,
			this.squareSize - 5
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

	getPossibleMoves = (event: MouseEvent): void => {
		const { left, top } = this.canvas.getBoundingClientRect();
		const row = Math.floor((event.clientY - top) / this.squareSize);
		const col = Math.floor((event.clientX - left) / this.squareSize);
		const { row: flippedRow, col: flippedCol } = this.flipPosition(row, col);
		this.possibleMoves = this.gamePtr.calcAndGetLegalMoves(flippedRow, flippedCol);
		this.currentSquare = { row: flippedRow, col: flippedCol };
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

	drawLiftedPiece = (event: MouseEvent, image: HTMLImageElement): void => {
		const { left, top } = this.canvas.getBoundingClientRect();
		const centerToMouse = 50;
		const row = (event.clientY - top - centerToMouse) / this.squareSize;
		const col = (event.clientX - left - centerToMouse) / this.squareSize;
		this.context.globalAlpha = 0.4;
		this.context.drawImage(
			image,
			this.squareSize * col + 2.5,
			this.squareSize * row + 5,
			this.squareSize - 5,
			this.squareSize - 5
		);
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

	getSelectedSquare = (event: MouseEvent): void => {
		const { left, top } = this.canvas.getBoundingClientRect();
		const r = Math.floor((event.clientY - top) / this.squareSize);
		const c = Math.floor((event.clientX - left) / this.squareSize);
		const { row: flippedRow, col: flippedCol } = this.flipPosition(r, c);
		for (let i = 0; i < this.possibleMoves.size(); i++) {
			const { row, col } = this.possibleMoves.get(i);
			if (flippedRow === row && flippedCol === col) {
				this.selectedSquare = { row, col };
				return;
			}
		}
		this.selectedSquare = null;
	};

	onMouseDown = (event: MouseEvent): void => {
		this.getPossibleMoves(event);
		if (this.possibleMoves.size() > 0) {
			const {
				piece: { type, color }
			} = this.squares.get(this.currentSquare.row).get(this.currentSquare.col);
			this.drawPossibleMoves(type, color);

			this.mouseDown = true;
		}
	};

	onMouseMove = (event: MouseEvent): void => {
		if (this.mouseDown) {
			this.getSelectedSquare(event);

			this.drawBoard();

			if (this.selectedSquare) {
				this.highlightSquare();
			}

			const {
				piece: { type, color }
			} = this.squares.get(this.currentSquare.row).get(this.currentSquare.col);

			this.drawPossibleMoves(type, color);

			const piece = getImage(type, color, this.images);

			this.drawLiftedPiece(event, piece);
		}
	};

	onMouseUp = (event: MouseEvent): void => {
		if (this.mouseDown) {
			const { left, top } = this.canvas.getBoundingClientRect();

			const row = Math.floor((event.clientY - top) / this.squareSize);
			const col = Math.floor((event.clientX - left) / this.squareSize);
			const { row: flippedRow, col: flippedCol } = this.flipPosition(row, col);
			if (flippedRow === this.selectedSquare?.row && flippedCol == this.selectedSquare?.col) {
				const isPawn =
					this.squares.get(this.currentSquare.row).get(this.currentSquare.col).piece.type ===
					'Pawn';
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
			}
			this.currentSquare = null;
			this.mouseDown = false;
			this.drawBoard();
		}
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
}

export default Board;
