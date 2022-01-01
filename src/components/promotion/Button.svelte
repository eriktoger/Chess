<script lang="ts">
	import { onChoosePromotion } from '../../stores/modals';
	export let src: string;
	export let alt: string;
	export let promoteTo: string;
</script>

<button class="imageButton" on:click={() => $onChoosePromotion(promoteTo)}>
	<img class="image" {src} {alt} />
</button>

<style>
	.image {
		height: 50px;
		width: 50px;
	}

	.imageButton:hover {
		background-color: rgba(0, 0, 0, 0.2);
	}
</style>
