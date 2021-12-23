import type { Images, RowArray, SquareArray } from 'src/types/chess';
import { getImage } from './helpers';

export const drawPiece = (
	context: CanvasRenderingContext2D,
	squareSize: number,
	image: HTMLImageElement,
	row: number,
	col: number
): void => {
	context.drawImage(
		image,
		squareSize * col + 2.5,
		squareSize * row + 5,
		squareSize - 5,
		squareSize - 5
	);
};

export const drawLiftedPiece = (
	event: MouseEvent,
	canvas: HTMLCanvasElement,
	context: CanvasRenderingContext2D,
	squareSize: number,
	image: HTMLImageElement
): void => {
	const { left, top } = canvas.getBoundingClientRect();
	const centerToMouse = 50;
	const row = (event.clientY - top - centerToMouse) / squareSize;
	const col = (event.clientX - left - centerToMouse) / squareSize;
	context.globalAlpha = 0.4;
	context.drawImage(
		image,
		squareSize * col + 2.5,
		squareSize * row + 5,
		squareSize - 5,
		squareSize - 5
	);
};

export const drawBoard = (
	context: CanvasRenderingContext2D,
	squares: RowArray,
	squareSize: number,
	images: Images,
	currentSquare?: { row: number; col: number }
): void => {
	for (let r = 0; r < squares.size(); r++) {
		for (let c = 0; c < squares.get(r).size(); c++) {
			const {
				piece: { color: pieceColor, type },
				row,
				col,
				color: squareColor
			} = squares.get(r).get(c);
			context.fillStyle = squareColor === 'White' ? 'white' : 'grey';
			context.fillRect(col * squareSize, row * squareSize, squareSize, squareSize);

			if (row === currentSquare?.row && col == currentSquare?.col) {
				context.globalAlpha = 0.4;
			} else {
				context.globalAlpha = 1;
			}
			const image = getImage(type, pieceColor, images);
			image && drawPiece(context, squareSize, image, row, col);
		}
	}
};

export const drawPossibleMoves = (
	context: CanvasRenderingContext2D,
	squares: RowArray,
	possibleMoves: SquareArray,
	squareSize: number,
	type: string,
	color: string,
	images: Images
): void => {
	for (let i = 0; i < possibleMoves.size(); i++) {
		const image = getImage(type, color, images);
		const { row, col } = possibleMoves.get(i);
		const square = squares.get(row).get(col);
		if (square.piece.type !== '') {
			context.fillStyle = 'red';
			context.globalAlpha = 0.25;
			context.fillRect(square.col * squareSize, square.row * squareSize, squareSize, squareSize);
		} else {
			context.globalAlpha = 0.4;
			drawPiece(context, squareSize, image, row, col);
		}
	}
};

export const highlightSquare = (
	context: CanvasRenderingContext2D,
	squareSize: number,
	row: number,
	col: number
): void => {
	context.beginPath();
	context.lineWidth = 4;
	context.rect(col * squareSize, row * squareSize, squareSize, squareSize);
	context.stroke();
};
