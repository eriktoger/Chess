import { writable } from 'svelte/store';

export const setPlayerPerspective = writable((perspective: string) => {
	console.log('Game not loaded', perspective);
});

export const createNewGame = writable(() => {
	console.log('Game not loaded');
});
