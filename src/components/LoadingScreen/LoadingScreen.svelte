<script lang="ts">
	import Spinner from './spinner.svelte';
</script>

<div class="overlay">
	<Spinner />
</div>

<style>
	.overlay {
		background: #fff;
		position: fixed;
		bottom: 0;
		right: 0;
		left: 0;
		top: 0;
		z-index: 10;
		display: flex;
		justify-content: center;
		align-items: center;
	}
</style>
