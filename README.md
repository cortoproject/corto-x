# x
The corto/x project is a line-based regex based parser that invokes rules when their patterns match the input. The project relies heavily on the corto typesystem to automatically generate types & code for the rules. Example usecases for corto/x are:
- match each line in a logfile against a set of actions
- match HTTP requests against a set of routes

### The format
Because regular expressions are a pain to read and write, and keeping a long list of regular expressions to match, for example, a logfile would be a hell to maintain, `corto/x` introduces a syntax that allows you to write human-readable regular expressions. An example:
```
x/token id: "[a-zA-Z][a-zA-Z_0-9]*"
x/token number: "[0-9]", type=int32

x/pattern time: "{sec:number}.{nanosec:number}"
x/pattern header: "{:time} {component:id}"

x/parser myParser::
  x/rule ruleOne: "{:header} this is a line in a logfile called {name:([a-z])}"
  x/rule ruleTwo: "{:header} this is rule number two"
```
Here's what it means:

type | description
------|------------
x/token | a reusable regular expression that you can use in `x/pattern` and `x/rule`
x/pattern | a reusable composite pattern that may consist of multiple `x/token`'s and `x/pattern`'s. Everything outside `{}` is considered plain text.
x/rule | equivalent to an `x/pattern` except that this will be matched against the input and an `x/pattern` won't. An `x/rule` must always appear in the scope of an `x/parser`.
x/parser | A collection of `x/rule`'s

And the `x/pattern` syntax:

syntax | description
-------|------------
`{name:type}` | A subexpression identified by `name` and of type `type`
`{type}` | An unnamed subexpression (data won't be available after parsing).
`{:type}` | A subexpression that will have the same name as the type. Equivalent to `{type:type}`.
`{(expr)}` | An inline regular (sub)expression
`{name:(expr)}` | A named inline regular (sub)expression

Note that a type always refers to an `x/token` or `x/rule`, never to an `x/rule`.

### Code generation
Think of `x/token`'s as primitive types, and `x/pattern` as composite types, because that is actually what they are. The `x/pattern` "time" and "header" from the example above translate into the following types:
```
struct time::
  sec, nanosec: int32
 
struct header::
  time: time
  component: string
```

If `x` definitions are part of a definition file, code will be generate for the tokens, patterns and rules. The rule implementations will be able to access data from the regular expression through an object that is passed as a parameter to the rule.

### The optimizer
The x parser automatically optimizes itself based on the provided regular expressions. For example, if you have three rules with the following regular expressions:
- rule1: `"Hello [A-z]"`
- rule2: `"Hello [A-z] times [0-9]"`
- rule3: `"Hello!!!"`
the parser will build a parser tree that looks like this:
```
- "Hello"
  - "!!!": rule3
  - [A-z]
     - "": rule1
     - "times [0-9]": rule2
```
This ensures that common parts in regular expressions are never evaluated more than once per input line, which allows for very fast parsing.


