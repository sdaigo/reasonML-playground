/* produce the sum of all the numbers in a list up to
 * but not including the first negative number
 */
let nums = [1, 2, 3, 4, 5, 5, 6, 7, 8];

/* with reduce, it would require going through every item in the list */
let rec sumUntilNegative = (items: list(int), total: int): int =>
  switch (items) {
  | [] => total
  | [x, ..._] when x < 0 => total
  | [x, ...xs] => sumUntilNegative(xs, total + x)
  };

Js.log(sumUntilNegative(nums, 0));

/* Display list as string */
let items = [10, 11, 12, 14, 14, 15];
let floatItems = [3.6, 7.9, 8.25, 41.0];

let stringOfList = (items: list('a), stringify: 'a => string): string => {
  let rec helper = (acc: string, theList: list('a)) =>
    switch (theList) {
    | [] => acc
    | [x] => acc ++ stringify(x)
    | [x, ...xs] => helper(acc ++ stringify(x) ++ ", ", xs)
    };

  helper("[", items) ++ "]";
};

Js.log(stringOfList(items, string_of_int));
Js.log(stringOfList(floatItems, string_of_float));