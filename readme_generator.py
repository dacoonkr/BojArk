from __future__ import annotations

import re
from pathlib import Path


SRC_DIR = Path("src")
PROBLEM_FILE = Path("problem.txt")
README_FILE = Path("README.md")

MIN_TIER = 16
MAX_TIER = 28

TIER_ICON_URL = "https://static.solved.ac/tier_small/{tier}.svg"


def load_problem_data() -> dict[int, int]:
    """
    problem.txt:
        문제번호 티어

    예:
        2430 25
        33006 28
    """

    if not PROBLEM_FILE.exists():
        raise FileNotFoundError(
            f"{PROBLEM_FILE} 파일이 없습니다."
        )

    problems: dict[int, int] = {}

    with PROBLEM_FILE.open("r", encoding="utf-8") as f:
        for line_number, line in enumerate(f, 1):
            line = line.strip()

            if not line:
                continue

            parts = line.split()

            if len(parts) != 2:
                print(
                    f"[경고] {PROBLEM_FILE}:{line_number}: "
                    f"잘못된 형식 -> {line}"
                )
                continue

            try:
                problem_id = int(parts[0])
                level = int(parts[1])
            except ValueError:
                print(
                    f"[경고] {PROBLEM_FILE}:{line_number}: "
                    f"숫자가 아님 -> {line}"
                )
                continue

            problems[problem_id] = level

    return problems


def extract_problem_id(filename: str) -> int | None:
    """
    파일명 앞쪽의 숫자를 문제 번호로 사용한다.

    예:
        33006.cpp      -> 33006
        34594-P1.cpp   -> 34594
        KOI2026-산책로.cpp -> None
    """

    match = re.match(r"^(\d+)", filename)

    if not match:
        return None

    return int(match.group(1))


def get_link_name(path: Path, problem_id: int | None) -> str:
    """
    README 링크에 표시할 이름.
    """

    stem = path.stem

    if problem_id is not None:
        return str(problem_id)

    # 특수 파일은 마지막 '-' 뒤를 표시
    if "-" in stem:
        return stem.rsplit("-", 1)[-1]

    return stem


def collect_sources() -> list[Path]:
    """
    src 아래 모든 cpp 파일 검색.
    """

    if not SRC_DIR.exists():
        raise FileNotFoundError(
            f"{SRC_DIR} 디렉터리가 없습니다."
        )

    return sorted(
        SRC_DIR.rglob("*.cpp"),
        key=lambda p: p.name.lower(),
    )


def build_rows(
    problem_data: dict[int, int],
) -> dict[int, list[Path]]:
    """
    src의 cpp 파일을 티어별로 분류한다.
    """

    rows: dict[int, list[Path]] = {
        tier: []
        for tier in range(MAX_TIER, MIN_TIER - 1, -1)
    }

    unknown_files: list[Path] = []
    missing_problem_data: list[tuple[Path, int]] = []
    out_of_range: list[tuple[Path, int, int]] = []

    for path in collect_sources():
        problem_id = extract_problem_id(path.name)

        if problem_id is None:
            unknown_files.append(path)
            continue

        if problem_id not in problem_data:
            missing_problem_data.append(
                (path, problem_id)
            )
            continue

        level = problem_data[problem_id]

        if level not in rows:
            out_of_range.append(
                (path, problem_id, level)
            )
            continue

        rows[level].append(path)

    for paths in rows.values():
        paths.sort(key=lambda p: p.name.lower())

    if unknown_files:
        print()
        print("[참고] 문제 번호를 자동으로 찾을 수 없는 파일:")

        for path in unknown_files:
            print(f"  - {path}")

    if missing_problem_data:
        print()
        print("[경고] problem.txt에 없는 문제:")

        for path, problem_id in missing_problem_data:
            print(f"  - {path} -> {problem_id}")

    if out_of_range:
        print()
        print("[참고] README 범위(16~28) 밖의 문제:")

        for path, problem_id, level in out_of_range:
            print(
                f"  - {path} -> {problem_id}, level={level}"
            )

    return rows


def make_markdown(rows: dict[int, list[Path]]) -> str:
    """
    README 전체 내용을 생성한다.
    """

    lines = [
        "# 아직 제출안한 백준 소스코드 모음",
        "",
        "백준이 돌아오면 제출할 소스코드를 난이도별로 정리한 저장소입니다.",
        "",
        "| 티어 | 소스코드 |",
        "|:---:|:---|",
    ]

    for tier in range(MAX_TIER, MIN_TIER - 1, -1):
        icon = (
            f'<img src="{TIER_ICON_URL.format(tier=tier)}" width="24">'
        )

        links: list[str] = []

        for path in rows[tier]:
            problem_id = extract_problem_id(path.name)
            link_name = get_link_name(path, problem_id)

            relative_path = path.as_posix()

            links.append(
                f"[{link_name}]({relative_path})"
            )

        source_column = " ".join(links)

        lines.append(
            f"| {icon} | {source_column} |"
        )

    return "\n".join(lines) + "\n"


def main() -> None:
    print("problem.txt 읽는 중...")
    problem_data = load_problem_data()

    print(f"문제 데이터 {len(problem_data):,}개 로드 완료.")

    print("src 스캔 중...")
    rows = build_rows(problem_data)

    print("README 생성 중...")
    content = make_markdown(rows)

    with README_FILE.open(
        "w",
        encoding="utf-8",
        newline="\n",
    ) as f:
        f.write(content)

    print()
    print("README.md 생성 완료.")
    print(f"위치: {README_FILE.resolve()}")

    print()
    print("문제 수:")

    for tier in range(MAX_TIER, MIN_TIER - 1, -1):
        print(
            f"  {tier:2d}: {len(rows[tier]):3d}"
        )


if __name__ == "__main__":
    main()