const CopyWebpackPlugin = require('copy-webpack-plugin')
const HTMLWebpackPlugin = require('html-webpack-plugin')

module.exports = {
    mode: 'development',
    devServer: {
        static: 'dist',
        port: 3000
    },
    devtool: 'inline-source-map',
    plugins: [
        new CopyWebpackPlugin({
            patterns: [{
                to: 'assets',
                from: 'assets'
            }]
        }),
        new HTMLWebpackPlugin({
            template: 'index.html',
            filename: 'index.html'
        })
    ]
}
