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
	const blackRook = await loadImage('/black_rook.png');
	const whiteRook = await loadImage('/white_rook.png');
	const blackQueen = await loadImage('/black_queen.png');
	const whiteQueen = await loadImage('/white_queen.png');
	return {
		blackPawn,
		whitePawn,
		blackBishop,
		whiteBishop,
		blackKnight,
		whiteKnight,
		blackRook,
		whiteRook,
		blackQueen,
		whiteQueen
	};
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
	if (type == 'Rook' && color === 'White') {
		return images.whiteRook;
	}
	if (type == 'Rook' && color === 'Black') {
		return images.blackRook;
	}
	if (type == 'Queen' && color === 'White') {
		return images.whiteQueen;
	}
	if (type == 'Queen' && color === 'Black') {
		return images.blackQueen;
	}
};
