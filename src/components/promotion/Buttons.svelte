<script lang="ts">
	import PromotionButton from './Button.svelte';
	export let isWhite: boolean;
</script>

<div>
	<PromotionButton
		src={isWhite ? 'white_queen.png' : 'black_queen.png'}
		alt={isWhite ? 'White Queen' : 'Black Queen'}
		promoteTo="Queen"
	/>

	<PromotionButton
		src={isWhite ? 'white_rook.png' : 'black_rook.png'}
		alt={isWhite ? 'White Rook' : 'Black Rook'}
		promoteTo="Rook"
	/>

	<PromotionButton
		src={isWhite ? 'white_bishop.png' : 'black_bishop.png'}
		alt={isWhite ? 'White Bishop' : 'Black Bishop'}
		promoteTo="Bishop"
	/>

	<PromotionButton
		src={isWhite ? 'white_knight.png' : 'black_knight.png'}
		alt={isWhite ? 'White Knight' : 'Black Knight'}
		promoteTo="Knight"
	/>
</div>

<style>
</style>
