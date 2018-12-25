let commaSplit = (s: string): array(string) => {
  let pattern = [%re "/\\s*, \\s*/"];
  Js.String.splitByRe(pattern, s);
};

type order = (int, ShirtSize.t);
type resultOrder = Belt.Result.t(order, string);