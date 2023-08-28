#! /usr/bin/nodejs
let spawn = require("await-spawn");

(async function asd() {
  let sec_items = [];
  let n_cities = ["10", "20", "30", "40", "50"];

  //  n_cities; n_args; alpha; beta; evaporation_rate; cycles;
  for (const i of n_cities) {
    let values = [i, "5", "1", "1", "0.5", "100"];
    let test = await spawn("./ants", values);
    sec_items.push(parseInt(test.toString("utf-8")));
  }

  //This will be piped into python plotter
  console.log(JSON.stringify({ sec_items, n_cities }));
})();
