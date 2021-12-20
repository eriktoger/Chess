import type { Coordinate } from '../types/chess';

export const initializeBoard = () => {
	const canvas = <HTMLCanvasElement>document.getElementById('canvas');
	const squareSize = Math.min(window.innerHeight - 300, window.innerWidth - 100) / 8;
	canvas.height = squareSize * 8;
	canvas.width = squareSize * 8;
	const context = canvas.getContext('2d');
	return { canvas, squareSize, context };
};

export const getPossibleMoves = (event, canvas, Module, squareSize): [Coordinate, any] => {
	const { left, top } = canvas.getBoundingClientRect();
	const row = Math.floor((event.clientY - top) / squareSize);
	const col = Math.floor((event.clientX - left) / squareSize);
	const getPossibleMoves = Module.getPossibleMoves(row, col);

	return [{ row, col }, getPossibleMoves];
};

export const getSelectedSquare = (canvas, event, squareSize, possibleMoves) => {
	const { left, top } = canvas.getBoundingClientRect();
	const r = Math.floor((event.clientY - top) / squareSize);
	const c = Math.floor((event.clientX - left) / squareSize);

	for (let i = 0; i < possibleMoves.size(); i++) {
		const { row, col } = possibleMoves.get(i);
		if (r === row && c === col) {
			return { row, col };
		}
	}
	return null;
};
