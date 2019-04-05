from typing import List
import ctypes
import argparse


class WthorHeader(ctypes.Structure):
    """
    header:
        date: 4
        num_games: 4
        misc_data: 4
        padding 4
    """
    _fields_ = (
        ('century', ctypes.c_byte),
        ('year', ctypes.c_byte),
        ('month', ctypes.c_byte),
        ('day', ctypes.c_byte),
        ('num_games', ctypes.c_int32),
        ('num_contest', ctypes.c_int16),
        ('year2', ctypes.c_int16),
        ('padding', ctypes.c_int32)
    )


class WthorStructure(ctypes.Structure):
    """
    contest index: 2
    black id: 2
    white id: 2
    black result: 1
    maximum result: 1
    move seq: 1*60
    """
    _fields_ = (
        ('contest_index', ctypes.c_int16),
        ('black_id', ctypes.c_int16),
        ('white_id', ctypes.c_int16),
        ('black_result', ctypes.c_int8),
        ('maximum_result', ctypes.c_int8),
        ('moves', ctypes.c_int8 * 60)
    )


def parse_moves(moves: List[ctypes.c_int8]) -> List[str]:
    def parse_move(move: ctypes.c_int8) -> str:
        x = move % 10
        y = move // 10
        assert 1 <= x <= 8 and 1 <= y <= 8
        x_begin = 'a'
        y_begin = '1'

        return '{}{}'.format(*[chr(ord(beg) + (z - 1))
                               for z, beg in zip([x, y], [x_begin, y_begin])])
    num_valid = 0
    for move in moves:
        # invalid data is represented by 0
        if move == 0:
            break
        num_valid += 1
    return [parse_move(move) for move in moves[:num_valid]]


def main() -> None:
    """A short example for parsing wthor files.

    ffo_wthor/ data is found at: http://www.ffothello.org/informatique/la-base-wthor/
    ggs_wthor/ data is found at: https://www.skatgame.net/mburo/ggs/game-archive/Othello/ (ggf files)
    A ggf-to-wthor converter is at https://www.skatgame.net/mburo/ggs/ .
    Wthor structure is described in http;//hp.vector.co.jp/authors/VA015468/platina/algo/append_a.html .
    """

    parser = argparse.ArgumentParser()
    parser.add_argument('file', help='a file to be parsed')
    parser.add_argument('--show-num', type=int, default=10,
                        help='# of files to be printed. default: %(default)s')
    args = parser.parse_args()

    h = WthorHeader()
    with open(args.file, 'rb') as f:
        f.readinto(h)
        print(h.century, h.year, h.month, h.day, h.num_games, h.num_contest, h.year2)
        print("Total game number : " + str(h.num_games))
        for i in range(h.num_games):
            t = WthorStructure()
            f.readinto(t)
            print(t.contest_index, t.black_id, t.white_id)
            moves = parse_moves(t.moves)
            result = t.black_result, len(moves) + 4 - t.black_result
            best = t.maximum_result, len(moves) + 4 - t.maximum_result
            print('[{}]\n -> B{}:{}W (Best B{}:{}W)'
                  .format(', '.join(moves), *result, *best))
            if i >= args.show_num:
                break


if __name__ == '__main__':
    main()
