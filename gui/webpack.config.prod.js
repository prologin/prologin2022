const CopyWebpackPlugin = require('copy-webpack-plugin')
const { ESBuildMinifyPlugin } = require('esbuild-loader')

module.exports = {
    mode: 'production',
    module: {
        rules: [{
            test: /\.(js)$/,
            exclude: /node_modules/,
            use: {
                loader: 'babel-loader'
            }
        }]
    },
    optimization: {
        minimizer: [
            new ESBuildMinifyPlugin({
                keepNames: true,
            }),
        ]
    },
    plugins: [
        new CopyWebpackPlugin({
            patterns: [{
                to: 'assets',
                from: 'assets'
            }]
        }),
    ]
}
