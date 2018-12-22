let example = [10, 11, 12];
Js.log(example);

let items = [10, 11, 12, 13, 14, 15, 16];
let sqrtPlusOne = x => sqrt(float_of_int(x) +. 1.0);
let roots = Belt.List.makeBy(5, sqrtPlusOne);

Js.log(roots);

let added1 = Belt.List.add(items, 9);
let added2 = [9, ...items];

Js.log(added1);
Js.log(added2);

let more = [17, 18, 19];
let joined = Belt.List.concat(items, more);

Js.log(joined);

let take3 = Belt.List.take(_, 3);
let drop3 = Belt.List.drop(_, 3);
let badTake = Belt.List.take(items, 100);

Js.log(take3(items));
Js.log(drop3(items));
Js.log(badTake);

let result = Belt.List.splitAt(items, 3);
switch (result) {
| Some((firstPart, lastPart)) =>
  Js.log(firstPart);
  Js.log(lastPart);
| None => Js.log("None")
};

let optElement = Belt.List.get(items, 3); /* Some(...) */
let badOptElement = Belt.List.get(items, 10); /* None */

/* option typeを返さない */
let element: int = Belt.List.getExn(items, 3);
let badElement: int =
  try (Belt.List.getExn(items, 10)) {
  | Js.Exn.Error(e) =>
    switch (Js.Exn.message(e)) {
    | Some(message) => Js.log({j|Error: $message|j})
    | None => Js.log("An unknown error occured")
    };
    (-1);
  };

Js.log(badElement);