# J-DIF

Convert LDIF (LDAP Data Interchange Format) to JSON.

## Getting Started

To compile run `make`.

To test an example run `make example`.

## Usage

### From stdin

By default, J-DIF will read LDIF content from standard input.

```bash
$ echo 'hello: world' | jdif 
{"hello": "world"}
```

### From file

If `-f` or `--file` is specified, J-DIF will parse an LDIF file.

```bash
$ jdif -f hello.txt
{"hello": "world"}
```

### Output format

Currently, J-DIF ouputs JSON inline, but it can be combined with other JSON formatting and parsing libraries like `jq`.


```bash
$ echo 'hello: world' | jdif | jq
{
  "hello": "world"
}
```
