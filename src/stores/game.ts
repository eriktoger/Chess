import { writable } from 'svelte/store';

export const setPlayerPerspective = writable((perspective: string) => {
	console.log('Game not loaded', perspective);
});

export const createNewGame = writable((timePerMove: number, useOpeningBook: boolean) => {
	console.log('Game not loaded', timePerMove, useOpeningBook);
});

export const usingTouch = writable(true);
