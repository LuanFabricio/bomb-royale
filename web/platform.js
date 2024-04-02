export default class Platform {

	/**
	* @param {CanvasRenderingContext2D} ctx 
	* @param {WebAssembly} ctx 
	* */
	constructor(ctx) {
		this.ctx = ctx;
		this.buffer = undefined;
		this.function_table = undefined;
		this.width = 0;
		this.height = 0;
		this.last_frame = Date.now();

		this.key_map = new Set

		document.addEventListener('keydown', (key) => {
			let key_code = key_to_code(key);
			this.key_map[key_code] = true;
		});

		document.addEventListener('keyup', (key) => {
			let key_code = key_to_code(key);
			this.key_map[key_code] = false;
		});
	}

	/**
	* @param {number} str_ptr 
	* @param {number} w 
	* @param {number} h 
	* */
	Platform_init_window(w, h, str_ptr) {
		this.width = w;
		this.height = h;

		this.ctx.canvas.width = this.width;
		this.ctx.canvas.height = this.height;
		document.title = cstr_by_ptr(this.buffer, str_ptr);
	}

	/** @param {number} color  */
	Platform_clear_background(color) {
		this.ctx.fillStyle = color_to_hex(color);
		this.ctx.clearRect(0, 0, this.width, this.height);
		this.ctx.fillRect(0, 0, this.width, this.height);
	}

	/**
	* @param {number} min 
	* @param {number} max 
	* */
	Platform_rand_range(min, max) {
		return min + Math.random() * (max - min);
	}

	/** @param {Number} fn_ptr */
	Platform_set_loop(fn_ptr) {
		this.loop_fn = this.func_table.get(fn_ptr);
	}

	/**
	* @param {number} x 
	* @param {number} y
	* @param {number} radius
	* @param {number} color
	* */
	Platform_draw_point(x, y, radius, color) {
		this.ctx.beginPath();
		this.ctx.arc(x, y, radius, 0, 2 * Math.PI, false);
		this.ctx.fillStyle = color_to_hex(color);
		this.ctx.fill();
	}
	/**
	* @param {number} x 
	* @param {number} y
	* @param {number} w
	* @param {number} h
	* @param {number} color
	* */
	Platform_draw_rectangle(x, y, w, h, color) {
		this.ctx.fillStyle = color_to_hex(color);
		this.ctx.fillRect(x, y, w, h);
	}

	/**
	* @param {number} mem_usage 
	* */
	Platform_log_mem(mem_usage) {
		console.info(`Mem usage: ${(mem_usage * 100).toFixed(2)}%`);
	}

	/**
	* @returns {number}
	* */
	Platform_get_frame_time() {
		return (Date.now() - this.last_frame) / 1000;
	}

	/**
	* @param {number} x 
	* @returns {number}
	* */
	Platform_sqrtf(x) {
		return Math.sqrt(x);
	}

	/**
	* @param {number} key 
	* @returns {boolean}
	* */
	Platform_is_key_down(key) {
		return this.key_map[key] ?? false;
	}

	Platform_rand_seed() {}
	Platform_begin_drawing() {}
	Platform_end_drawing() {
		this.last_frame = Date.now();
	}
	Platform_close_drawing() {}
	Platform_window_should_close() { return true; }
}

/**
* @param {KeyboardEvent} key 
* */
function key_to_code(key) {
	const keyMapping = {
		"Enter": 257,
	};
	
	let code = keyMapping[key.code];
	if (code) {
		return code;
	}

	return key.key.toUpperCase().charCodeAt();
}


/**
* @param {number} color 
* */
function color_to_hex(color) {
	const r = ((color>>(0*8))&0xff).toString(16).padStart(2, "0");
	const g = ((color>>(1*8))&0xff).toString(16).padStart(2, "0");
	const b = ((color>>(2*8))&0xff).toString(16).padStart(2, "0");
	const a = ((color>>(3*8))&0xff).toString(16).padStart(2, "0");
	return '#'+r+g+b+a;
}

/**
* @param {ArrayBuffer} buffer 
* @param {number} str_ptr 
* @returns {string}
* */
function cstr_by_ptr(buffer, str_ptr) { 
	const mem = new Uint8Array(buffer);
	const len = cstr_len(mem, str_ptr);
	const str_bytes = new Uint8Array(buffer, str_ptr, len);

	return new TextDecoder().decode(str_bytes);
}

/**
* @param {Uint8Array} buffer 
* @param {number} str_ptr 
* @returns {number}
* */
function cstr_len(mem, str_ptr) {
	let len = 0;
	while (mem[str_ptr++] != 0) {
		len++;
	}
	return len;
}
