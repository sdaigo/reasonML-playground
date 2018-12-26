module ColorComparator =
  Belt.Id.MakeComparable({
    type t = Shirt.Color.t;
    let cmp = compare;
  });

type colorMapType = Belt.Map.t(Shirt.Color.t, int, ColorComparator.identity);

let optFloat = (str: string): option(float) =>
  switch (float_of_string(str)) {
  | n => Some(n)
  | exception (Failure("float_of_string")) => None
  };

let optInt = (str: string): option(int) =>
  switch (int_of_string(str)) {
  | n => Some(n)
  | exception (Failure("int_of_string")) => None
  };

let colorReducer = (acc: colorMapType, item: Shirt.order): colorMapType => {
  let n = Belt.Map.getWithDefault(acc, item.color, 0);
  Belt.Map.set(acc, item.color, n + item.quantity);
};

let printStatistics = (orders: list(Shirt.order)): unit => {
  let colorDistribution =
    Belt.List.reduce(
      orders,
      Belt.Map.make(~id=(module ColorComparator)),
      colorReducer,
    );
  Js.log2("Color", "Quantity");

  Belt.Map.forEach(colorDistribution, (key, value) =>
    Js.log2(Shirt.Color.toString(key), value)
  );
};

let map2 = (optX, optY, f) =>
  switch (optX, optY) {
  | (Some(x), Some(y)) => Some(f(x, y))
  | (_, _) => None
  };

let lineReducer = (acc: list(Shirt.order), line: string): list(Shirt.order) => {
  let items = Js.String.split(",", line);
  Js.log(items);
  if (Belt.Array.length(items) != 7) {
    acc;
  } else {
    let initial: option(Shirt.order) =
      Some({
        quantity: 0,
        size: Shirt.Size.Small,
        color: Shirt.Color.White,
        pattern: Shirt.Pattern.Solid,
        collar: Shirt.Collar.Straight,
        sleeve: Shirt.Sleeve.Short,
        cuff: Shirt.Cuff.Button,
      });

    let orderRecord =
      map2(initial, optInt(items[0]), (result, n) =>
        {...result, quantity: n}
      )
      ->map2(Shirt.Size.fromString(items[1]), (result, size) =>
          {...result, size}
        )
      ->map2(Shirt.Color.fromString(items[2]), (result, color) =>
          {...result, color}
        )
      ->map2(Shirt.Pattern.fromString(items[3]), (result, pattern) =>
          {...result, pattern}
        )
      ->map2(Shirt.Collar.fromString(items[4]), (result, collar) =>
          {...result, collar}
        )
      ->map2(Shirt.Sleeve.fromString(items[5]), (result, sleeve) =>
          {...result, sleeve}
        )
      ->map2(Shirt.Cuff.fromString(items[6]), (result, cuff) =>
          {...result, cuff}
        );

    switch (orderRecord) {
    | Some(result) => [result, ...acc]
    | None => acc
    };
  };
};

let processFile = (fileName: string): unit => {
  let fileContents = Node.Fs.readFileAsUtf8Sync(fileName);
  /* get rid of header line */
  let lines = Js.String.split("\n", fileContents)->Belt.Array.sliceToEnd(1);

  let orders = Belt.Array.reduce(lines, [], lineReducer);
  printStatistics(orders);
};

let nodeArg = Belt.Array.get(Node.Process.argv, 0);
let progArg = Belt.Array.get(Node.Process.argv, 1);
let fileArg = Belt.Array.get(Node.Process.argv, 2);

switch (nodeArg, progArg, fileArg) {
| (_, _, Some(inFileName)) => processFile(inFileName)
| (Some(node), Some(prog), _) =>
  Js.log("Usage: " ++ node ++ " " ++ prog ++ " input-file.csv")
| (_, _, _) =>
  Js.log("How did you get here without NodeJS or a program to run?")
};