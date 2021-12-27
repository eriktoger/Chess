import type { BoardPosition, SquareArray, Images, RowArray, TModule } from '../types/chess';
import { getImage } from '../functions/helpers';

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

	constructor(Module: TModule, images: Images) {
		this.Module = Module;
		this.squares = this.Module.getSquares();

		this.images = images;

		this.canvas = <HTMLCanvasElement>document.getElementById('canvas');
		this.squareSize = Math.min(window.innerHeight - 300, window.innerWidth - 100) / 8;
		this.canvas.height = this.squareSize * 8;
		this.canvas.width = this.squareSize * 8;
		this.context = this.canvas.getContext('2d');

		this.canvas.addEventListener('mousedown', (event) => {
			this.onMouseDown(event);
		});

		this.canvas.addEventListener('mousemove', (event) => {
			this.onMouseMove(event);
		});

		this.canvas.addEventListener('mouseup', (event) => {
			this.onMouseUp(event);
		});

		this.drawBoard();
	}

	drawPiece = (image: HTMLImageElement, row: number, col: number): void => {
		this.context.drawImage(
			image,
			this.squareSize * col + 2.5,
			this.squareSize * row + 5,
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
				this.context.fillRect(
					col * this.squareSize,
					row * this.squareSize,
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
			}
		}
	};

	getPossibleMoves = (event: MouseEvent): void => {
		const { left, top } = this.canvas.getBoundingClientRect();
		const row = Math.floor((event.clientY - top) / this.squareSize);
		const col = Math.floor((event.clientX - left) / this.squareSize);
		this.possibleMoves = this.Module.getPossibleMoves(row, col);
		this.currentSquare = { row, col };
	};

	drawPossibleMoves = (type: string, color: string): void => {
		for (let i = 0; i < this.possibleMoves.size(); i++) {
			const image = getImage(type, color, this.images);
			const { row, col } = this.possibleMoves.get(i);
			const square = this.squares.get(row).get(col);
			if (square.piece.type !== '') {
				this.context.fillStyle = 'red';
				this.context.globalAlpha = 0.25;
				this.context.fillRect(
					square.col * this.squareSize,
					square.row * this.squareSize,
					this.squareSize,
					this.squareSize
				);
			} else {
				this.context.globalAlpha = 0.4;
				this.drawPiece(image, row, col);
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
	};

	highlightSquare = (): void => {
		this.context.beginPath();
		this.context.lineWidth = 4;
		this.context.rect(
			this.selectedSquare?.col * this.squareSize,
			this.selectedSquare?.row * this.squareSize,
			this.squareSize,
			this.squareSize
		);
		this.context.stroke();
	};

	getSelectedSquare = (event: MouseEvent): void => {
		const { left, top } = this.canvas.getBoundingClientRect();
		const r = Math.floor((event.clientY - top) / this.squareSize);
		const c = Math.floor((event.clientX - left) / this.squareSize);

		for (let i = 0; i < this.possibleMoves.size(); i++) {
			const { row, col } = this.possibleMoves.get(i);
			if (r === row && c === col) {
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

			const piece = getImage(type, color, this.images); //make class method?

			this.drawLiftedPiece(event, piece);
		}
	};

	onMouseUp = (event: MouseEvent): void => {
		if (this.mouseDown) {
			const { left, top } = this.canvas.getBoundingClientRect();
			const row = Math.floor((event.clientY - top) / this.squareSize);
			const col = Math.floor((event.clientX - left) / this.squareSize);

			if (row === this.selectedSquare?.row && col == this.selectedSquare?.col) {
				this.squares = this.Module.movePiece(
					this.currentSquare.row,
					this.currentSquare.col,
					this.selectedSquare.row,
					this.selectedSquare.col
				);
			}
			this.currentSquare = null;
			this.mouseDown = false;

			this.drawBoard();
		}
	};
}

export default Board;
