export type BoardPosition = {
	row: number;
	col: number;
};
export type Piece = {
	color: string;
	type: string;
	value: number;
};

export type Square = {
	row: number;
	col: number;
	piece: Piece;
	color: string; //These can only be "Black", "White", ""
};

export type SquareArray = {
	get: (i: number) => Square;
	size: () => number;
};

export type RowArray = {
	get: (i: number) => SquareArray;
	size: () => number;
};

export type GamePtr = {
	calcAndGetLegalMoves: (row: number, col: number) => SquareArray;
	getSquares: () => RowArray;
	makeAMove: (
		startRow: number,
		startCol: number,
		endRow: number,
		endCol: number
	) => { status: string; squares: RowArray };
	getTurn: () => string;
	setPromotionType: (type: string) => void;
	newGame: (playerColor: string) => void;
	makeComputerMove: () => { status: string; squares: RowArray };
};

export type TModule = {
	onRuntimeInitialized: () => void;
	getGame: () => GamePtr;
};

export type Images = {
	whitePawn: HTMLImageElement;
	blackPawn: HTMLImageElement;
	whiteBishop: HTMLImageElement;
	blackBishop: HTMLImageElement;
	whiteKnight: HTMLImageElement;
	blackKnight: HTMLImageElement;
	whiteRook: HTMLImageElement;
	blackRook: HTMLImageElement;
	whiteQueen: HTMLImageElement;
	blackQueen: HTMLImageElement;
	whiteKing: HTMLImageElement;
	blackKing: HTMLImageElement;
};
