/** @type {import('tailwindcss').Config} */
module.exports = {
  content: ["./src/**/*.{html,js}"],
  theme: {
    fontFamily: {
      retro: ["VT323", "monospace"],
      code: ["Fira Code", "monospace"],
    },
    colors: {
      red:'#FF1000',
      green:'#00FF00',
      space:'#0F0F0F',
      white:'#FFFFFF',
      yellow:{
        '100':'#FFF8E5',
        '200':'#FFEFCB',
        '300':'#FFE6B1',
        '400':'#FFDD97',
        '500':'#FFD47D',
        '600':'#FFCB63',
        '700':'#FFC24A',
        '800':'#FFB930',
        '900':'#FFB017',
      },
      gray:{
        '100':'#F3F3F3',
        '200':'#E0E0E0',
        '300':'#C8C8C8',
        '400':'#AFAFAF',
        '500':'#969696',
        '600':'#7D7D7D',
        '700':'#646464',
        '800':'#4B4B4B',
        '900':'#323232',
      }
    },
  },
  // plugins: [],
}

