#include "VideoReceiver.h"
#include "mainwindow.h"

class
    SetPlaying : public QRunnable
{
public:
    SetPlaying(GstElement *);
    ~SetPlaying();

    void run();

private:
    GstElement *pipeline_;
};

SetPlaying::SetPlaying(GstElement *pipeline)
{
    pipeline_ = pipeline ? static_cast<GstElement *>(gst_object_ref(pipeline)) : nullptr;
}

SetPlaying::~SetPlaying()
{
    // if (pipeline_)
    // gst_object_unref(pipeline_);
}

void SetPlaying::run()
{
    if (pipeline_)
    {
        gst_element_set_state(pipeline_, GST_STATE_PLAYING);
    }
}

VideoReceiver::VideoReceiver(QObject *parent, bool verticalFlip)
    : QObject(parent),
      _recordingElement(new RecordingElement),
      _starting(false),
      _playing(false),
      _pausing(false),
      _recStarting(false),
      _recording(false),
      _recStoping(false),
      _verticalFlip(verticalFlip),
      _videoWidth(0),
      _videoHeight(0),
      _videoSize(0),
      _videoDataRaw(nullptr),
      _videoMap(new GstMapInfo),
      _videoBuffer(nullptr)
{
    _pipeline = gst_pipeline_new("receiver");
    _teeRecording = gst_element_factory_make("tee", "tee-recording");
    g_assert(_pipeline && _teeRecording);

    connect(this, &VideoReceiver::pipelineEOS, this, &VideoReceiver::onPipelineEOS);
}

VideoReceiver::~VideoReceiver()
{
    delete _recordingElement;
    delete _videoMap;

    g_object_unref(_pipeline);
    g_object_unref(_teeRecording);
}

void VideoReceiver::start(QQuickWidget *quickWidget)
{
    if (_starting)
    {
        qDebug() << "staring";
    }

    GstElement *src = gst_element_factory_make("udpsrc", "udpsrc");
    GstElement *demux = gst_element_factory_make("rtph264depay", "rtp-h264-depacketizer");
    GstElement *parser = gst_element_factory_make("h264parse", "h264-parser");
    GstElement *queue = gst_element_factory_make("queue", "queue-recording-main");
    GstElement *decoder = gst_element_factory_make("avdec_h264", "h264-decoder");
    GstElement *videoflip = gst_element_factory_make("videoflip", "vertical-flip");
    GstElement *queue2 = gst_element_factory_make("queue", "queue-cv-main");
    GstElement *glupload = gst_element_factory_make("glupload", "glupload");
    GstElement *glcolorconvert = gst_element_factory_make("glcolorconvert", "glcolorconvert");
    GstElement *qmlsink = gst_element_factory_make("qmlglsink",NULL);
    /* the plugin must be loaded before loading the qml file to register the
     * GstGLVideoItem qml item */
    
    g_assert(src && demux && parser && queue && decoder && videoflip &&
             queue2 && glupload && glcolorconvert && qmlsink);

    GstCaps *caps = nullptr;
    if ((caps = gst_caps_from_string(
             "application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264")) == nullptr)
    {
        qCritical() << "Error with gst_caps_from_string()";
    }

    g_object_set(G_OBJECT(src), "caps", caps, nullptr);
    g_object_set(G_OBJECT(src), "port", 5600, nullptr); // set UDP port

    if (_verticalFlip)
    {
        g_object_set(videoflip, "video-direction", 5, nullptr);
    }


    gst_bin_add_many(GST_BIN(_pipeline), src, demux, parser, _teeRecording, queue, decoder,
                     videoflip, glupload, glcolorconvert, qmlsink,
                     nullptr);
    if (!gst_element_link_many(src, demux, parser, _teeRecording, queue, decoder,
                               videoflip, glupload, glcolorconvert, qmlsink,
                               nullptr))
    {
        qCritical() << "failed to link elements";
    }


    assert(quickWidget);
    QUrl qmlSource("qrc:/assets/video.qml");
    quickWidget->setSource(qmlSource);

    QQuickItem *videoItem;
    QQuickWindow *qQuickWindows;

    /* find and set the videoItem on the sink */
    videoItem = quickWidget->rootObject()->findChild<QQuickItem *>("videoItem");
    assert(videoItem);
    g_object_set(qmlsink, "widget", videoItem, nullptr);

    qQuickWindows = videoItem->window();
    assert(qQuickWindows);
    qQuickWindows->scheduleRenderJob(new SetPlaying(_pipeline),
                                     QQuickWindow::BeforeSynchronizingStage);

    GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(_pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "pipeline");

    // Add handler for EOS event
    GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(_pipeline));
    gst_bus_enable_sync_message_emission(bus);
    g_signal_connect(bus, "sync-message", G_CALLBACK(_onBusMessage),
                     this);
    gst_object_unref(bus);
    bus = nullptr;

    _starting = true;
}

void VideoReceiver::play()
{
    if (_pausing)
    {
        gst_element_set_state(_pipeline, GST_STATE_PLAYING);
    }
}

void VideoReceiver::pause()
{
    if (_playing)
    {
        gst_element_set_state(_pipeline, GST_STATE_NULL);
    }

    if (_recording)
    {
    }

    _pausing = true;
}

bool VideoReceiver::startRecording()
{
    if (!_playing)
    {
        qDebug() << "no video stream!";
        return false;
    }

    if (_recording)
    {
        qDebug() << "in recording processing!";
        return false;
    }

    if (_recStarting)
    {
        qDebug() << "in recStarting processing!";
        return false;
    }

    _recStarting = true;

    _pipelineStopRec = gst_pipeline_new("pipelineStopRec");

    GstElement *queue = gst_element_factory_make("queue", "queue-recording");
    GstElement *parse = gst_element_factory_make("h264parse", "h264-parser-recording");
    GstElement *mux = gst_element_factory_make("matroskamux", "matroska-mux");
    GstElement *filesink = gst_element_factory_make("filesink", "mkv-filesink");

    g_assert(_pipelineStopRec && queue && parse && mux && filesink);

    g_object_set(static_cast<gpointer>(mux), "streamable", false, nullptr);
    g_object_set(static_cast<gpointer>(mux), "writing-app", qPrintable("SubControl"), nullptr);

    QString videoFile = "";
    videoFile += QDateTime::currentDateTime().toString("yyyy-MM-dd_hh.mm.ss") + ".mkv";

    g_object_set(static_cast<gpointer>(filesink), "location", qPrintable(videoFile), nullptr);

    gst_bin_add_many(GST_BIN(_pipeline), queue, parse, mux, nullptr);
    gst_element_link_many(queue, parse, mux, nullptr);

    gst_element_sync_state_with_parent(queue);
    gst_element_sync_state_with_parent(parse);
    gst_element_sync_state_with_parent(mux);

    _recordingElement->pThis = this;
    _recordingElement->pipeline = _pipeline;
    _recordingElement->pipelineStopRec = _pipelineStopRec;
    _recordingElement->tee = _teeRecording;
    _recordingElement->queue = queue;
    _recordingElement->parse = parse;
    _recordingElement->mux = mux;
    _recordingElement->sink = filesink;
    _recordingElement->removing = false;

    // Install a probe on the recording branch to drop buffers until we hit our first keyframe.
    // When we hit our first keyframe,
    // we can offset the timestamps appropriately according to the first keyframe time
    // This will ensure the first frame is a keyframe at t=0,
    // and decoding can begin immediately on playback
    // Once we have this valid frame, we attach the filesink.
    // Attaching it here would cause the filesink to fail to preroll
    // and to stall the pipeline for a few seconds.
    GstPad *probepad = gst_element_get_static_pad(queue, "src");
    gst_pad_add_probe(probepad,
                      (GstPadProbeType)(GST_PAD_PROBE_TYPE_BUFFER),
                      /* | GST_PAD_PROBE_TYPE_BLOCK */
                      _keyframeWatch, _recordingElement, nullptr);
    // to drop the buffer or to block the buffer?
    gst_object_unref(probepad);

    // Link the recording branch to the pipeline
    GstPad *teepad = gst_element_get_request_pad(_teeRecording, "src_%u");
    GstPad *sinkpad = gst_element_get_static_pad(queue, "sink");
    g_assert(teepad && sinkpad);

    if (gst_pad_link(teepad, sinkpad) != GST_PAD_LINK_OK)
    {
        qCritical() << "link failed!";
    }

    gst_object_unref(sinkpad);

    GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(_pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "pipeline-recording");

    return true;
}

void VideoReceiver::stopRecording()
{
    // exit immediately if we are not recording
    if (!_playing || !_recording)
    {
        qDebug() << "Not recording!";
        return;
    }

    if (_recStoping)
    {
        qDebug() << "in recStoping processing!";
        return;
    }

    _recStoping = true;

    // Wait for data block before unlinking
    gst_pad_add_probe(gst_element_get_request_pad(_teeRecording, "src_%u"),
                      GST_PAD_PROBE_TYPE_IDLE, _unlinkCallBack, _recordingElement, nullptr);
    // _recording = false;
    // emit onRecordingChanged();
}

GstPadProbeReturn VideoReceiver::_keyframeWatch(GstPad *pad,
                                                GstPadProbeInfo *info,
                                                gpointer user_data)
{
    Q_UNUSED(pad);
    if (info != nullptr && user_data != nullptr)
    {
        GstBuffer *buf = gst_pad_probe_info_get_buffer(info);
        if (GST_BUFFER_FLAG_IS_SET(buf, GST_BUFFER_FLAG_DELTA_UNIT))
        { // wait for a keyframe
            return GST_PAD_PROBE_DROP;
        }
        else
        {
            struct RecordingElement *recordingElement = (struct RecordingElement *)user_data;
            // reset the clock
            GstClock *clock = gst_pipeline_get_clock(GST_PIPELINE(recordingElement->pipeline));
            GstClockTime time = gst_clock_get_time(clock);
            gst_object_unref(clock);
            gst_element_set_base_time(recordingElement->pipeline, time);
            // offset pipeline timestamps to start at zero again
            buf->dts = 0;
            // The offset will not apply to this current buffer, our first frame, timestamp is zero
            buf->pts = 0;

            // Add the filesink once we have a valid I-frame
            gst_bin_add_many(GST_BIN(recordingElement->pipeline), recordingElement->sink, nullptr);
            gst_element_link_many(recordingElement->mux, recordingElement->sink, nullptr);
            gst_element_sync_state_with_parent(recordingElement->sink);

            recordingElement->pThis->_recording = true;
            recordingElement->pThis->_recStarting = false;
            emit recordingElement->pThis->onRecordingChanged();

            qDebug() << "Got keyframe, stop dropping buffers";
        }
    }

    return GST_PAD_PROBE_REMOVE;
}

GstPadProbeReturn
VideoReceiver::_unlinkCallBack(GstPad *pad, GstPadProbeInfo *info, gpointer user_data)
{
    Q_UNUSED(pad);

    if (info != nullptr && user_data != nullptr)
    {
        RecordingElement *recordingElement = (RecordingElement *)user_data;
        // We will only act once
        if (g_atomic_int_compare_and_exchange(&recordingElement->removing, false, true))
        {
            // unlinks
            gst_bin_remove_many(GST_BIN(recordingElement->pipeline),
                                recordingElement->queue,
                                recordingElement->parse, recordingElement->mux,
                                recordingElement->sink, nullptr);

            // Give tee its pad back
            GstPad *teepad = gst_element_get_request_pad(recordingElement->tee, "src_%u");
            gst_element_release_request_pad(recordingElement->tee, teepad);
            gst_object_unref(teepad);
            teepad = nullptr;

            // Create temporary pipeline
            recordingElement->pipelineStopRec = gst_pipeline_new("pipeStopRec");

            // Put our elements from the recording branch into the temporary pipeline
            gst_bin_add_many(GST_BIN(recordingElement->pipelineStopRec),
                             recordingElement->queue,
                             recordingElement->parse,
                             recordingElement->mux,
                             recordingElement->sink, nullptr);
            gst_element_link_many(recordingElement->queue,
                                  recordingElement->parse,
                                  recordingElement->mux,
                                  recordingElement->sink, nullptr);

            // Add handler for EOS event
            GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(recordingElement->pipelineStopRec));
            gst_bus_enable_sync_message_emission(bus);
            g_signal_connect(bus, "sync-message", G_CALLBACK(_onBusMessage),
                             recordingElement->pThis);
            gst_object_unref(bus);
            bus = nullptr;

            if (gst_element_set_state(recordingElement->pipelineStopRec, GST_STATE_PLAYING) ==
                GST_STATE_CHANGE_FAILURE)
            {
                qDebug() << "problem starting _pipelineStopRec";
            }

            // Send EOS at the beginning of the pipeline
            GstPad *sinkpad = gst_element_get_static_pad(recordingElement->queue, "sink");
            gst_pad_send_event(sinkpad, gst_event_new_eos());
            gst_object_unref(sinkpad);
            sinkpad = nullptr;
            qDebug() << "Recording branch unlinked";
        }
    }
    return GST_PAD_PROBE_REMOVE;
}

gboolean VideoReceiver::_onBusMessage(GstBus *bus, GstMessage *msg, gpointer data)
{
    Q_UNUSED(bus)
    Q_ASSERT(msg != nullptr && data != nullptr);
    VideoReceiver *pThis = (VideoReceiver *)data;

    switch (GST_MESSAGE_TYPE(msg))
    {

    case (GST_MESSAGE_ERROR):
    {
        gchar *debug;
        GError *error;
        gst_message_parse_error(msg, &error, &debug);
        g_free(debug);
        qCritical() << error->message;
        g_error_free(error);
        qDebug() << "GStreamer error!";
        // pThis->stop();
    }
    break;

    case (GST_MESSAGE_EOS):

        // do clean work in slot
        emit pThis->pipelineEOS();

        break;

    case (GST_MESSAGE_STATE_CHANGED):

        if (pThis->_pipeline &&
            pThis->_playing != (GST_STATE(pThis->_pipeline) == GST_STATE_PLAYING))
        {
            pThis->_playing = GST_STATE(pThis->_pipeline) == GST_STATE_PLAYING;
            qDebug() << "State changed, _playing:" << pThis->_playing;
        }
        break;

    default:
        break;
    }

    return TRUE;
}

void VideoReceiver::setRecordingHightlight(bool hightlight)
{
    _mouseOverRecordingButton = hightlight;
    emit onMouseOverRecordingButtonChanged();
}

void VideoReceiver::onPipelineEOS()
{
    if (_recStoping)
    {
        gst_bin_remove(GST_BIN(_recordingElement->pipelineStopRec), _recordingElement->queue);
        gst_bin_remove(GST_BIN(_recordingElement->pipelineStopRec), _recordingElement->parse);
        gst_bin_remove(GST_BIN(_recordingElement->pipelineStopRec), _recordingElement->mux);
        gst_bin_remove(GST_BIN(_recordingElement->pipelineStopRec), _recordingElement->sink);

        gst_element_set_state(_recordingElement->pipelineStopRec, GST_STATE_NULL);
        gst_object_unref(_recordingElement->pipelineStopRec);
        _recordingElement->pipelineStopRec = nullptr;

        gst_element_set_state(_recordingElement->sink, GST_STATE_NULL);
        gst_element_set_state(_recordingElement->parse, GST_STATE_NULL);
        gst_element_set_state(_recordingElement->mux, GST_STATE_NULL);
        gst_element_set_state(_recordingElement->queue, GST_STATE_NULL);

        gst_object_unref(_recordingElement->queue);
        gst_object_unref(_recordingElement->parse);
        gst_object_unref(_recordingElement->mux);
        gst_object_unref(_recordingElement->sink);

        _recording = false;
        emit onRecordingChanged(); // for qml button

        _recStoping = false;

        qDebug() << "Recording Stopped.";
    }
}
