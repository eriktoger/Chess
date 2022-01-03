<script lang="ts">
	import { modalPlacement } from '../../stores/modals';
</script>

<div
	id="myModal"
	class="modal"
	style=" --left:{$modalPlacement.left}px;
            --top: {$modalPlacement.top}px;
            --width: {$modalPlacement.width}px"
>
	<div class="modal-content">
		<slot />
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
