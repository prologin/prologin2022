const CopyWebpackPlugin = require('copy-webpack-plugin')
const TerserJSPlugin = require('terser-webpack-plugin')

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
        minimizer: [new TerserJSPlugin({
            extractComments: false,
        })]
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
