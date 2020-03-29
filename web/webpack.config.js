const path = require('path')
const HtmlWebpackPlugin = require('html-webpack-plugin')

const isProduction = process.env.NODE_ENV === "production";

module.exports = {
  entry: './src/Index.bs.js',
  mode: 'production',
  output: {
    path: path.join(__dirname, 'dist'),
    filename: 'app.js'
  },
  plugins: [
    new HtmlWebpackPlugin({
      template: 'template.ejs'
    })
  ],
    devServer: {
        proxy: !isProduction 
        ?  {
            '/api': 'http://localhost:3030'
          }
        : undefined
    }
}
