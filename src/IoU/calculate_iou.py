
import os
import pandas as pd
from collections import defaultdict
from numpy import asarray
from typing import DefaultDict, List, Any, Union, Dict


class CALCULATE_IOU:

    data: DefaultDict[str, List[Union[float, str, Any]]]
    gt: Dict[str, List[Union[float, str, Any]]]
    IOU: DefaultDict[str, float]

    def __init__(self, path_name):
        self.data = defaultdict(list)
        self.gt = defaultdict(list)
        self.IOU = defaultdict(float)

        if (path_name.endswith("/")):
            files = [path_name + file for file in os.listdir(path_name) if (file.endswith(".txt"))]
            self.load_images(files)
            # print("Data -> ", self.data)
            # print("Ground Truth ->", self.gt)
            # print("IOU ->  ", self.IOU)

    def load_images(self, path_names):
        for i in range(len(path_names)):
            name = path_names[i].split(".")[0] + ".txt"
            with open(name, "r") as file:
                lines = file.read(1)
                if not lines:
                    continue
                else:
                    liness = file.readlines()
                    for x in range(len(liness)):
                        if (x == 0):
                            if (liness[x] != ""):
                                local = liness[x].split(" ")
                                if local:
                                    self.gt[name].append([0. if x == "" else int(x) for x in local[1:]])
                        else:
                            if (liness[x] != ""):
                                local = liness[x].split(" ")
                                if local:
                                    self.data[name].append([0. if x == "" else int(x) for x in local[1:]])

            file.close()
            self.calc_IOU(name)

    @staticmethod
    def _bb_intersection_over_union(boxA, boxB, name=None):
        # boxA_x1, boxA_y1, boxA_x2, boxA_y2 = boxA
        # boxB_x3, boxB_y3, boxB_x4, boxB_y4 = boxB
        # determine the (x, y)-coordinates of the intersection rectangle
        xA = max(boxA[0], boxB[0])
        yA = max(boxA[1], boxB[1])
        xB = min(boxA[2], boxB[2])
        yB = min(boxA[3], boxB[3])

        # compute the area of intersection rectangle
        interArea = max(0, xB - xA + 1) * max(0, yB - yA + 1)
        if (interArea == 0):
            return 0.0
        # compute the area of both the prediction and ground-truth rectangles
        boxAArea = abs(boxA[2] - boxA[0]) * abs(boxA[3] - boxA[1])
        boxBArea = abs(boxB[2] - boxB[0]) * abs(boxB[3] - boxB[1])

        # compute the intersection over union by taking the intersection area and dividing it by
        # the sum of prediction + ground-truth areas - the intersection area
        # print("intersection area ->", interArea, float(boxAArea + boxBArea - interArea))
        iou = interArea / float(boxAArea + boxBArea - interArea)

        # return the intersection over union value
        if (iou < 0 or iou > 1):
            return 0.0
        return iou

    def calc_IOU(self, name):
        bbox = self.data[name]
        gt_bbox = self.gt[name]
        name = name.split("/")[-1]
        name = name.split(".")[0]
        if (len(bbox) > 1):
            for i in range(len(bbox)):
                if (name.find("__") != -1):
                    name = name.split("__")[0]
                    name = name + "__" + str(i)
                else:
                    name = name + "__" + str(i)
                    # print("NAME -> ", name)
                try:
                    # print("Box -> ", gt_bbox, bbox)
                    iou = self._bb_intersection_over_union(asarray(gt_bbox[0]), asarray(bbox[i]), name=name)
                except IndexError as e:
                    print("Info[INDEX ERROR] : ", e)
                    iou = 0.0
                self.IOU[name] = iou
        else:
            iou = self._bb_intersection_over_union(asarray(gt_bbox[0]), asarray(bbox[0]), name=name)
            self.IOU[name] = iou

    def make_dataframe(self):

        self.df = pd.DataFrame.from_dict(self.IOU, orient="index", columns=['IOU'])
        # self.df["Ground Truth"] = list(self.GT.values())
        # self.df["IOU > 0.5"] = self.df.apply(self.select_IOU_value, axis=1)
        # self.df["TP"] = self._truePositive(self.df)
        # self.df["FP"] =self._falsePositive(self.df)
        # self.df["FN"] =self._falseNegative(self.df)
        # self.df["Pr"] = self.calc_precision(self.df, rc=True)
        # self.df["Rc"] = self.calc_precision(self.df, rc=False)
        # self.saveCSV("data.csv")
        print(self.df.tail(60))


if __name__ == "__main__":
    pass
