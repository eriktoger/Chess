import type { BoardPosition, SquareArray, Images, RowArray, TModule } from '../types/chess';
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
	Module: TModule;
	mouseDown = false;
	currentSquare: BoardPosition;
	possibleMoves: SquareArray;
	selectedSquare: BoardPosition;
	images: Images;
	squares: RowArray;
	playerPerspective: string;

	constructor(Module: TModule, images: Images) {
		this.Module = Module;
		this.squares = this.Module.getSquares();
		this.images = images;
		this.playerPerspective = 'White';

		this.canvas = <HTMLCanvasElement>document.getElementById('canvas');

		this.context = this.canvas.getContext('2d');

		this.resizeBoard();

		setPlayerPerspective.set((perspective) => {
			this.playerPerspective = perspective;
		});

		createNewGame.set(() => {
			this.Module.newGame();
			this.squares = this.Module.getSquares();
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

	drawPiece = (image: HTMLImageElement, row: number, col: number): void => {
		const { row: flippedRow, col: flippedCol } = this.flipPosition(row, col);

		this.context.drawImage(
			image,
			this.squareSize * flippedCol + 2.5,
			this.squareSize * flippedRow + 5,
			this.squareSize - 5,
			this.squareSize - 5
		);
	};

	drawBoard = (): void => {
		for (let r = 0; r < this.squares.size(); r++) {
			for (let c = 0; c < this.squares.get(r).size(); c++) {
				const {
					piece: { color: pieceColor, type },
					row,
					col,
					color: squareColor
				} = this.squares.get(r).get(c);
				this.context.fillStyle = squareColor === 'White' ? 'white' : 'grey';
				const { row: flippedRow, col: flippedCol } = this.flipPosition(row, col);
				this.context.fillRect(
					flippedCol * this.squareSize,
					flippedRow * this.squareSize,
					this.squareSize,
					this.squareSize
				);

				if (row === this.currentSquare?.row && col == this.currentSquare?.col) {
					this.context.globalAlpha = 0.4;
				} else {
					this.context.globalAlpha = 1;
				}
				const image = getImage(type, pieceColor, this.images);
				image && this.drawPiece(image, row, col);
				this.context.globalAlpha = 1;
			}
		}
	};

	getPossibleMoves = (event: MouseEvent): void => {
		const { left, top } = this.canvas.getBoundingClientRect();
		const row = Math.floor((event.clientY - top) / this.squareSize);
		const col = Math.floor((event.clientX - left) / this.squareSize);
		const { row: flippedRow, col: flippedCol } = this.flipPosition(row, col);
		this.possibleMoves = this.Module.getPossibleMoves(flippedRow, flippedCol);
		this.currentSquare = { row: flippedRow, col: flippedCol };
	};

	drawPossibleMoves = (type: string, color: string): void => {
		for (let i = 0; i < this.possibleMoves.size(); i++) {
			const image = getImage(type, color, this.images);
			const { row, col } = this.possibleMoves.get(i);
			const square = this.squares.get(row).get(col);

			if (square.piece.type !== '') {
				const { row: flippedRow, col: flippedCol } = this.flipPosition(row, col);
				this.context.fillStyle = 'red';
				this.context.globalAlpha = 0.25;
				this.context.fillRect(
					flippedCol * this.squareSize,
					flippedRow * this.squareSize,
					this.squareSize,
					this.squareSize
				);
			} else {
				this.context.globalAlpha = 0.4;
				this.drawPiece(image, row, col);
				this.context.globalAlpha = 1;
			}
		}
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
				const { status, squares } = this.Module.movePiece(
					this.currentSquare.row,
					this.currentSquare.col,
					this.selectedSquare.row,
					this.selectedSquare.col
				);
				this.squares = squares;
				gameStatus.set(status);
			}
			this.currentSquare = null;
			this.mouseDown = false;
			this.drawBoard();
		}
	};

	onPromotion = (): void => {
		promotionColor.set(this.Module.getTurn());

		onChoosePromotion.set((type: string) => {
			this.Module.setPromotionType(type);
			const { status, squares } = this.Module.movePiece(
				this.currentSquare.row,
				this.currentSquare.col,
				this.selectedSquare.row,
				this.selectedSquare.col
			);
			this.squares = squares;
			this.currentSquare = null;
			this.mouseDown = false;
			this.drawBoard();
			gameStatus.set(status);
			showPromotionModal.set(false);
		});

		showPromotionModal.set(true);
	};

	flipPosition = (row: number, col: number): BoardPosition =>
		this.playerPerspective === 'White' ? { row, col } : { row: 7 - row, col: 7 - col };
}

export default Board;
