import { writable } from 'svelte/store';

export const setPlayerPerspective = writable((perspective: string) => {
	console.log('Game not loaded', perspective);
});

export const createNewGame = writable((timePerMove: number) => {
	console.log('Game not loaded', timePerMove);
});

export const timePerMove = writable(5000);
