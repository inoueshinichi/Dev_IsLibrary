"""
utf8文字をutf16文字に変換
"""

import argparse
import codecs

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("input_utf8")
    parser.add_argument("output_utf16")

    args = parser.parse_args()

    with codecs.open(args.input_utf8, "r", "utf-8") as f_in:
        with codecs.open(args.output_utf16, "w", "utf-16") as f_out:
            for row in f_in:
                f_out.write(row)