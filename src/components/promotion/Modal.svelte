<script lang="ts">
	import { promotionModalPlacement, promotionColor } from '../../stores/modals';
	import PromotionButtons from './Buttons.svelte';
</script>

<div
	id="myModal"
	class="modal"
	style=" --left:{$promotionModalPlacement.left}px;
            --top: {$promotionModalPlacement.top}px;
            --width: {$promotionModalPlacement.width}px"
>
	<div class="modal-content">
		<p>Promote pawn to:</p>
		<PromotionButtons isWhite={$promotionColor === 'White'} />
	</div>
</div>

<style>
	.modal {
		display: flex;
		position: fixed;
		z-index: 1;
		left: var(--left);
		top: var(--top);
		width: var(--width);
		height: var(--width);
		background-color: rgba(0, 0, 0, 0.4);
	}

	.modal-content {
		background-color: #fefefe;
		margin: auto;
		padding: 20px;
		border: 1px solid #888;
		width: 300px;
	}
</style>
