import Module from '../wasm/chess.js';
import type { TModule } from '../types/chess';

export const getModule = (): TModule => {
	return Module;
};
