<script lang="ts">
	import type { Coordinate, SquareArray } from '../types/chess';

	import { onMount } from 'svelte';
	import {
		drawBoard,
		drawPossibleMoves,
		drawLiftedPiece,
		highlightSquare
	} from '../functions/draw';
	import { loadImages, getImage } from '../functions/helpers';
	import { initializeBoard, getPossibleMoves, getSelectedSquare } from '../functions/board';
	import { getModule } from '../functions/module';
	const Module = getModule();

	onMount(() => {
		const { canvas, context, squareSize } = initializeBoard();
		Module.onRuntimeInitialized = async () => {
			const images = await loadImages();

			let squares = Module.getSquares();
			drawBoard(context, squares, squareSize, images);

			let mouseDown = false;
			let currentSquare: Coordinate;
			let possibleMoves: SquareArray;
			let selectedSquare: Coordinate;

			canvas.addEventListener('mousedown', (event) => {
				[currentSquare, possibleMoves] = getPossibleMoves(event, canvas, Module, squareSize);
				if (possibleMoves.size() > 0) {
					const {
						piece: { type, color }
					} = squares.get(currentSquare.row).get(currentSquare.col);
					drawPossibleMoves(context, squares, possibleMoves, squareSize, type, color, images);

					mouseDown = true;
				}
			});

			canvas.addEventListener('mousemove', (event) => {
				if (mouseDown) {
					selectedSquare = getSelectedSquare(canvas, event, squareSize, possibleMoves);

					drawBoard(context, squares, squareSize, images, currentSquare);

					if (selectedSquare) {
						highlightSquare(context, squareSize, selectedSquare.row, selectedSquare.col);
					}

					const {
						piece: { type, color }
					} = squares.get(currentSquare.row).get(currentSquare.col);

					drawPossibleMoves(context, squares, possibleMoves, squareSize, type, color, images);

					const piece = getImage(type, color, images);
					drawLiftedPiece(event, canvas, context, squareSize, piece);
				}
			});

			canvas.addEventListener('mouseup', (event) => {
				if (mouseDown) {
					const { left, top } = canvas.getBoundingClientRect();
					const row = Math.floor((event.clientY - top) / squareSize);
					const col = Math.floor((event.clientX - left) / squareSize);

					if (row === selectedSquare?.row && col == selectedSquare?.col) {
						squares = Module.movePiece(
							currentSquare.row,
							currentSquare.col,
							selectedSquare.row,
							selectedSquare.col
						);
					}
					drawBoard(context, squares, squareSize, images);
					mouseDown = false;
				}
			});
		};
	});
</script>

<h1>Chess</h1>
<p>Welcome to my Chess page!</p>
<div>
	<canvas id="canvas" />
</div>

<style>
	#canvas {
		border: 4px dotted purple;
	}
</style>
