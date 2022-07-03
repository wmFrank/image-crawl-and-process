# Images crawling and processing

## Intro
This repo implements an image crawler with Python requests and a simple image processer with OpenCV.

## Workflow
- get html from web url with [requests](https://github.com/psf/requests)
- get image url with [regular expresstion](https://docs.python.org/3/library/re.html)
- get the images of the whole website with Depth-First Search(DFS)
- webpage deduplication with hash function and [Bloom filter](https://en.wikipedia.org/wiki/Bloom_filter)
- process the images with [OpenCV](https://github.com/opencv/opencv)

