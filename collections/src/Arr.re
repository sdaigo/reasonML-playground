let items = [|10, 11, 12, 13, 14, 15, 16|];
Js.log(items);

let more = [|17, 18, 19|];
let joined = Belt.Array.concat(items, more);

let part1 = [|"the", "array", "has"|];
let part2 = Belt.Array.concat(part1, [|"more"|]);
Js.log(part2);

Js.log(Belt.Array.slice(items, ~offset=1, ~len=3));

let arrayTake = (arr, n) => Belt.Array.slice(arr, ~offset=0, ~len=n);

let arrayDrop = (arr, n) =>
  Belt.Array.concat(
    Belt.Array.slice(arr, ~offset=0, ~len=n),
    Belt.Array.slice(arr, ~offset=n + 1, ~len=Belt.Array.length(arr)),
  );

let arraySplitAt = (arr, n) => [|
  Belt.Array.slice(arr, ~offset=0, ~len=n),
  Belt.Array.slice(arr, ~offset=n + 1, ~len=Belt.Array.length(arr)),
|];

Js.log(arrayTake(items, 3));
Js.log(arrayTake(items, 5));

Js.log(arrayDrop(items, 3));
Js.log(arrayDrop(items, 5));

Js.log(arraySplitAt(items, 3));
Js.log(arraySplitAt(items, 5));