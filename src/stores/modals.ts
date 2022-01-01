import { writable } from 'svelte/store';

export const showPromotionModal = writable(false);

export const promotionModalPlacement = writable({ left: 0, top: 0, width: 0 });

export const promotionColor = writable('White');

export const onChoosePromotion = writable((type: string) => {
	console.log('Promotion not choosen', type);
});
