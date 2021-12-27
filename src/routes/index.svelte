<script lang="ts">
	import { onMount } from 'svelte';
	import { loadImages } from '../functions/helpers';
	import { getModule } from '../functions/module';
	import Board from '../classes/board';

	const Module = getModule();

	onMount(() => {
		Module.onRuntimeInitialized = async () => {
			const images = await loadImages();
			new Board(Module, images);
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
