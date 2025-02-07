from .logFile import LogFile
from .statusStream import StatusStreamer
logger = LogFile()
logger.create_log_file()
statusStreamer = StatusStreamer()