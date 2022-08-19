from IoU.calculate_iou import CALCULATE_IOU
from os import path
import pytest


path = path.dirname(path.abspath(__file__))

@pytest.mark.parametrize("path", [
    (path)])
def test(path):
    CALCULATE_IOU(path)
