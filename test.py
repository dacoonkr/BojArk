from __future__ import annotations

import json
from pathlib import Path


INPUT_FILE = Path("problem.json")
OUTPUT_FILE = Path("problem.txt")


def main() -> None:
    if not INPUT_FILE.exists():
        raise FileNotFoundError(f"{INPUT_FILE} 파일이 없습니다.")

    with INPUT_FILE.open("r", encoding="utf-8") as f:
        data = json.load(f)

    problems: list[tuple[int, int]] = []

    for problem_id, info in data.items():
        try:
            problem_id_int = int(problem_id)
            level = int(info["level"])
        except (KeyError, TypeError, ValueError) as e:
            print(f"[경고] 잘못된 데이터: {problem_id} -> {info} ({e})")
            continue

        problems.append((problem_id_int, level))

    problems.sort()

    with OUTPUT_FILE.open("w", encoding="utf-8", newline="\n") as f:
        for problem_id, level in problems:
            f.write(f"{problem_id} {level}\n")

    print(f"변환 완료: {OUTPUT_FILE}")
    print(f"문제 수: {len(problems):,}")


if __name__ == "__main__":
    main()