const CopyWebpackPlugin = require('copy-webpack-plugin')
const RawBundlerPlugin = require('webpack-raw-bundler');

module.exports = {
    mode: 'production',
    plugins: [
        new CopyWebpackPlugin({
            patterns: [{
                to: 'assets',
                from: 'assets'
            }]
        }),
        new RawBundlerPlugin({
            bundles: [ "out.js" ],
            "out.js": [ 'src/*.js' ]
        })
    ]
}
