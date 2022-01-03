<script lang="ts">
	import { promotionColor } from '../../stores/modals';
	import PromotionButtons from './Buttons.svelte';
	import CenterModal from '../common/CenterModal.svelte';
</script>

<CenterModal>
	<p>Promote pawn to:</p>
	<PromotionButtons isWhite={$promotionColor === 'White'} />
</CenterModal>
