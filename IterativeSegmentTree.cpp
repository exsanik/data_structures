class SegmentTree {
   private:
    vector<vector<int64_t>> segmentTreeContainer;

   public:
    explicit SegmentTree(const vector<int64_t>& segmentTreeBase) {
        int rowLength = segmentTreeBase.size();
        const int logElements = __builtin_clz(rowLength); // get the postion of the highest "1" bit

        segmentTreeContainer.resize(logElements);
        segmentTreeContainer[0] = segmentTreeBase;
        for (int i = 1; i < logElements; ++i, rowLength /= 2) {
            segmentTreeContainer[i].resize(rowLength);
        }

        rowLength = segmentTreeBase.size();
        for (int rowIndex = 1; rowLength > 1; rowIndex++) {
            for (int columnIndex = 0; columnIndex < rowLength / 2;
                 columnIndex++) {
                segmentTreeContainer[rowIndex][columnIndex] = std::min(
                    segmentTreeContainer[rowIndex - 1][2 * columnIndex],
                    segmentTreeContainer[rowIndex - 1][2 * columnIndex + 1]);
            }
            rowLength /= 2;
        }
    }

    int64_t rangeMinimumQuery(const int leftBorder, const int rightBorder) {
        int currentLeftBorder = leftBorder;
        int currentRightBorder = rightBorder + 1;
        int64_t queryResult = INT64_MAX;
        int depthLevel = 0;
        while (currentLeftBorder != currentRightBorder) {
            if (currentLeftBorder % 2) {
                queryResult = std::min(
                    segmentTreeContainer[depthLevel][currentLeftBorder],
                    queryResult);
                currentLeftBorder++;
            }
            if (currentRightBorder % 2) {
                queryResult = std::min(
                    segmentTreeContainer[depthLevel][currentRightBorder],
                    queryResult);
            }

            currentLeftBorder >>= 1;
            currentRightBorder >>= 1;
            depthLevel++;
        }
        return queryResult;
    }
};
