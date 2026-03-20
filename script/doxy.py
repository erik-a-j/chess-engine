
def _description(t: str, title: str, text: str) -> list[str]:
    text_lines = text.splitlines()
    lines = [f"{t} * {title} {text_lines[0]}\n"]
    for i in range(1, len(text_lines)):
        lines.append(f"{t} * {' ' * len(title)} {text_lines[i]}\n")
    return lines

def doxygen(
        file: str | None = None,
        brief: str | None = None,
        desc: str | None = None,
        params: list[tuple[str, str]] = None,
        indent: int = 0) -> list[str]:

    t = " " * indent
    lines = [f"{t}/**\n", f"{t} */\n"]

    if file is not None:
        lines[-1:-1] = _description(t, "@file", file)
    if brief is not None:
        lines[-1:-1] = _description(t, "@brief", brief)
    if desc is not None:
        lines[-1:-1] = _description(t, "", desc)
    if params is not None:
        for param, desc in params:
            lines[-1:-1] = _description(t, f"@param {param}", desc)

    return lines

__all__ = ["doxygen"]