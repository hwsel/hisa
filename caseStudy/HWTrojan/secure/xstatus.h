#define XST_SUCCESS                     0L
#define XST_FAILURE                     1L
#define XST_DEVICE_NOT_FOUND            2L
#define XST_DEVICE_BLOCK_NOT_FOUND      3L
#define XST_INVALID_VERSION             4L
#define XST_DEVICE_IS_STARTED           5L
#define XST_DEVICE_IS_STOPPED           6L
#define XST_FIFO_ERROR                  7L
#define XST_INVALID_PARAM               15L
#define XST_NO_FEATURE                  19L
#define XST_DMA_ERROR                   9L
#define XST_DEVICE_BUSY                 21L
typedef int XStatus;
#define XST_DMA_SG_LIST_ERROR           526L
#define XST_FIFO_NO_ROOM                11L
#define XST_DMA_SG_NO_LIST              523L	/* no scatter gather list has
						   been created */
#define XST_DMA_SG_BD_NOT_COMMITTED     524L	/* the buffer descriptor which was
						   being started was not committed
						   to the list */
#define XST_DMA_SG_NO_DATA              525L	/* the buffer descriptor to start
						   has already been used by the
						   hardware so it can't be reused
						 */
#define XST_FIFO_NO_ROOM                11L
