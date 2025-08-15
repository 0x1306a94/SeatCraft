import resolve from '@rollup/plugin-node-resolve';
import commonJs from '@rollup/plugin-commonjs';
import json from '@rollup/plugin-json';
import { terser } from 'rollup-plugin-terser';
import esbuild from 'rollup-plugin-esbuild';


const plugins = [
    esbuild({
        tsconfig: 'tsconfig.json',
        minify: false,
    }),
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

const umdConfig = {
    input: 'src/binding.ts',
    output: [
        {
            name: 'seatcraft_app_picker',
            format: 'umd',
            exports: 'named',
            sourcemap: true,
            file: 'lib/seatcraft_app_picker.js',
        },
    ],
    plugins: plugins,
};

const umdMinConfig = {
    input: 'src/binding.ts',
    output: [
        {
            name: 'seatcraft_app_picker',
            format: 'umd',
            exports: 'named',
            sourcemap: true,
            file: 'lib/seatcraft_app_picker.min.js',
        },
    ],
    plugins: [
        ...plugins,
        terser(),
    ],
};

export default [
    umdConfig,
    umdMinConfig,
    {
        input: 'src/binding.ts',
        output: [
            { file: 'lib/seatcraft_app_picker.esm.js', format: 'esm', sourcemap: true },
            { file: 'lib/seatcraft_app_picker.cjs.js', format: 'cjs', exports: 'auto', sourcemap: true },
        ],
        plugins: plugins,
    },
];
