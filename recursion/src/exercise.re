let data = [2, 6, 42, 5, 7, 20, 3];
let isEven = n => n mod 2 === 0;

let takeWhile = (items: list('a), predict: 'a => bool): list('a) => {
  let rec helper = (items: list('a), acc: list('a)) =>
    switch (items) {
    | [] => Belt.List.reverse(acc)
    | [x, ...xs] =>
      predict(x) ? helper(xs, [x, ...acc]) : Belt.List.reverse(acc)
    };

  helper(items, []);
};

let dropWhile = (items: list('a), predict: 'a => bool): list('a) => {
  let rec helper = (items: list('a)) =>
    switch (items) {
    | [] => []
    | [x, ...xs] => predict(x) ? helper(xs) : items
    };

  helper(items);
};

let res1 = takeWhile(data, isEven);
let res2 = dropWhile(data, isEven);

Js.log2(res1 == [2, 6, 42], res1);
Js.log2(res2 == [5, 7, 20, 3], res2);