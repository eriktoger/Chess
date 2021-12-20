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
	return { blackPawn, whitePawn };
};

export const getImage = (type: string, color: string, images: Images): HTMLImageElement => {
	if (type == 'Pawn' && color === 'White') {
		return images.whitePawn;
	}
	if (type == 'Pawn' && color === 'Black') {
		return images.blackPawn;
	}
};
