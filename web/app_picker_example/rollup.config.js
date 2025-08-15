import copy from 'rollup-plugin-copy';
import esbuild from 'rollup-plugin-esbuild';
import resolve from '@rollup/plugin-node-resolve';
import commonJs from '@rollup/plugin-commonjs';
import json from '@rollup/plugin-json';

const plugins = [
    copy({
        targets: [
            { src: 'src/index.html', dest: 'dist/' },
            { src: '../out/SeatCraftAppPicker.js', dest: 'dist/wasm/' },
            { src: '../out/SeatCraftAppPicker.wasm', dest: 'dist/wasm/' },
            { src: 'resources/*', dest: 'dist/resources/' }
        ]
    }),
    esbuild({ tsconfig: "tsconfig.json", minify: false }),
    json(),
    resolve(),
    commonJs(),
    {
        name: 'preserve-import-meta-url',
        resolveImportMeta(property, options) {
            // Preserve the original behavior of `import.meta.url`.
            if (property === 'url') {
                return 'import.meta.url';
            }
            return null;
        },
    },
];

export default [
    {
        input: 'src/index.ts',
        output: {
            file: `dist/index.js`,
            format: 'esm',
            sourcemap: true
        },
        plugins: plugins,
    },
];