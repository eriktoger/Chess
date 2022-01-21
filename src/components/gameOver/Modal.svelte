<script lang="ts">
	import { gameStatus } from '../../stores/modals';
	import { setPlayerPerspective, createNewGame, timePerMove } from '../../stores/game';
	import CenterModal from '../common/CenterModal.svelte';

	let time = '5.0';
	let useOpeningBook = true;
	const onClick = (perspective: string) => {
		gameStatus.set('');
		$setPlayerPerspective(perspective);
		$createNewGame(Number(time) * 1000, useOpeningBook);
	};
</script>

<CenterModal>
	<div class="container">
		<h3 class="header">{$gameStatus}</h3>
		<div class="break" />
		<input
			autocomplete="off"
			type="range"
			min="0"
			max="10"
			step="0.1"
			on:input={(e) => (time = Number(e.currentTarget.value).toFixed(1))}
		/>

		<div class="break" />
		{#if $timePerMove === 0}
			<p>The computer will do random moves</p>
		{:else}
			<p>The computer will use {time} seconds per move</p>
		{/if}
		<div class="break" />
		<div class="openingBook">
			<input
				type="checkbox"
				checked={useOpeningBook}
				on:change={() => (useOpeningBook = !useOpeningBook)}
			/>
			<label for="scales">Computer will use an opening book</label>
		</div>
		<div class="break" />
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
	.openingBook {
		margin-bottom: 10px;
	}
</style>
