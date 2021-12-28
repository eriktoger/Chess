import type { Images } from 'src/types/chess';

export const loadImage = (url: string): Promise<HTMLImageElement> => {
	return new Promise((r) => {
		const image = new Image();
		image.onload = () => r(image);
		image.src = url;
	});
};

export const loadImages = async (): Promise<Images> => {
	const blackPawn = await loadImage('/black_pawn.png');
	const whitePawn = await loadImage('/white_pawn.png');
	const blackBishop = await loadImage('/black_bishop.png');
	const whiteBishop = await loadImage('/white_bishop.png');
	const blackKnight = await loadImage('/black_knight.png');
	const whiteKnight = await loadImage('/white_knight.png');
	return { blackPawn, whitePawn, blackBishop, whiteBishop, blackKnight, whiteKnight };
};

export const getImage = (type: string, color: string, images: Images): HTMLImageElement => {
	if (type == 'Pawn' && color === 'White') {
		return images.whitePawn;
	}
	if (type == 'Pawn' && color === 'Black') {
		return images.blackPawn;
	}
	if (type == 'Bishop' && color === 'White') {
		return images.whiteBishop;
	}
	if (type == 'Bishop' && color === 'Black') {
		return images.blackBishop;
	}
	if (type == 'Knight' && color === 'White') {
		return images.whiteKnight;
	}
	if (type == 'Knight' && color === 'Black') {
		return images.blackKnight;
	}
};
