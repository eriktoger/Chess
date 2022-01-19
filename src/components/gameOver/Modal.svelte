<script lang="ts">
	import { gameStatus } from '../../stores/modals';
	import { setPlayerPerspective, createNewGame, timePerMove } from '../../stores/game';
	import CenterModal from '../common/CenterModal.svelte';

	const onClick = (perspective: string) => {
		gameStatus.set('');
		$setPlayerPerspective(perspective);
		$createNewGame($timePerMove);
	};

	let time: string;

	timePerMove.subscribe((value) => {
		time = (value / 1000).toFixed(1);
	});
</script>

<CenterModal>
	<div class="container">
		<h3 class="header">{$gameStatus}</h3>
		<div class="break" />
		<input
			autocomplete="off"
			type="range"
			min="0"
			max="10000"
			on:input={(e) => timePerMove.set(Number(e.currentTarget.value))}
		/>
		{#if $timePerMove === 0}
			<p>The computer will do random moves</p>
		{:else}
			<p>The computer will use {time} seconds per move</p>
		{/if}
		<button class="button" on:click={() => onClick('White')}>Play as White</button>
		<button class="button" on:click={() => onClick('Black')}>Play as Black</button>
	</div>
</CenterModal>

<style>
	.container {
		display: flex;
		justify-content: space-around;
		flex-wrap: wrap;
		margin: 0;
	}
	.break {
		flex-basis: 100%;
		height: 0;
	}
	.button {
		padding: 10px;
	}
	.header {
		margin-top: 0;
	}
</style>
