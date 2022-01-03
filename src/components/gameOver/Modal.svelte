<script lang="ts">
	import { gameStatus } from '../../stores/modals';
	import CenterModal from '../common/CenterModal.svelte';
</script>

<CenterModal>
	<div class="container">
		<p>{$gameStatus}</p>
		<button>New Game</button>
	</div>
</CenterModal>

<style>
	.container {
		display: flex;
		justify-content: space-around;
	}
</style>
