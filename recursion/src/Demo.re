let rec isPalindrome = (s: string): bool => {
  let len = Js.String.length(s);
  if (len <= 1) {
    true;
  } else if (Js.String.get(s, 0) != Js.String.get(s, len - 1)) {
    false;
  } else {
    isPalindrome(Js.String.slice(~from=1, ~to_=len - 1, s));
  };
};

let repeatWithReduce = (s: string, n: int) =>
  Belt.Array.reduce(
    Belt.Array.range(0, n - 1), "", (acc: string, _item: int) =>
    acc ++ s
  );

let rec repeatRec = (s: string, acc: string, n: int): string =>
  switch (n) {
  | 0 => acc
  | _ => repeatRec(s, acc ++ s, n - 1)
  };

let repeat = (s: string, n: int) => {
  let rec repeatHelper = (acc: string, counter: int): string =>
    switch (counter) {
    | 0 => acc
    | _ => repeatHelper(acc ++ s, counter - 1)
    };

  repeatHelper("", n);
};

Js.log(repeat("ha", 4));

Js.log2("civic", isPalindrome("civic"));
Js.log2("deed", isPalindrome("deed"));
Js.log2("runner", isPalindrome("runner"));

let testString = repeat("a", 50000);
let rec repeatTest = (n: int, accTime: float): float =>
  switch (n) {
  | 0 => accTime
  | _ =>
    let startTime = Js.Date.now();
    let _ = isPalindrome(testString);
    let endTime = Js.Date.now();

    repeatTest(n - 1, accTime +. (endTime -. startTime));
  };

let totalTime = repeatTest(1000, 0.0);
Js.log2("Average time in msec: ", totalTime /. 1000.0);

/* keep only the indices of the matching item with keep function */
let keepIndices = (arr: array('a), f: 'a => bool): array(int) => {
  let rec helper = (position: int, acc: array(int)): array(int) =>
    if (position < Belt.Array.length(arr)) {
      f(Belt.Array.getUnsafe(arr, position)) ?
        helper(position + 1, Belt.Array.concat(acc, [|position|])) :
        helper(position + 1, acc);
    } else {
      acc;
    };

  helper(0, [||]);
};

let words = [|"cow", "aardvark", "squirrel", "fish", "snake", "capybara"|];

let isShortWord = s => Js.String.length(s) < 6;

let result = keepIndices(words, isShortWord);
Js.log(result);