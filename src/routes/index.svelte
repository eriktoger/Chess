<script lang="ts">
	import PromotionModal from '../components/promotion/Modal.svelte';
	import GameOverModal from '../components/gameOver/Modal.svelte';
	import { showPromotionModal, gameStatus } from '../stores/modals';
	import { onMount } from 'svelte';
	import { loadImages } from '../functions/helpers';
	import { getModule } from '../functions/module';
	import Board from '../classes/board';
	import LoadingScreen from '../components/LoadingScreen/LoadingScreen.svelte';

	const Module = getModule();
	let loading = true;
	onMount(() => {
		Module.onRuntimeInitialized = async () => {
			const images = await loadImages();
			const board = new Board(Module.getGame(), images);

			window.addEventListener('resize', () => {
				board.resizeBoard();
			});
			loading = false;
		};
	});
</script>

{#if loading}
	<LoadingScreen />
{/if}

{#if $showPromotionModal}
	<PromotionModal />
{/if}

{#if $gameStatus !== ''}
	<GameOverModal />
{/if}

<h1>Chess</h1>

<div class="container">
	<canvas id="canvas" />
</div>

<style>
	.container {
		border: 8px solid rgba(102, 19, 19);
		display: table;
	}
	#canvas {
		vertical-align: middle;
	}
</style>
