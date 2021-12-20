<script>
	import Module from '../wasm/chess.js';
	Module.onRuntimeInitialized = () => {
		console.log(Module.add(1, 2));
	};
</script>

<h1>Chess</h1>
<p>Welcome to my Chess page!</p>
