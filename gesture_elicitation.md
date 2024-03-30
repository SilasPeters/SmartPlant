# Gesture elicitation

## Context

To increase the input bandwidth and ease of use of our system, we will add
gestures to the input modalities. In order to properly implement the gesture
recognition setup, such that it is inviting to use with high usability, we will
need to conduct a proper research a prior.

Let us first define the referents. The referents include two basic gestures
which aim to trigger the most common interactions with our watering system:

- Watering the plant
- Reading the sensor values

### Intended use of gestures

We would like to recognize the 'water plant' gesture, to trigger the plant
watering system such that it waters the plant with a reasonable amount - even if
the moist readings of the system indicate that the soil is saturated with enough
water.

The 'read sensors' gesture must trigger the system to not only read the sensors
at instant, but also share the resulting measurements through all possible
media: the OLED and the MQTT network. The previously scheduled sensor readings
could be rescheduled such that it does not measure all sensors within a short
timespan again.

### Target audience

The target audience of our plant watering system, and thus our gesture
elicitation, consists of tech-savvy users of any age who have an understanding
of how the sensors of our system might operate. We expect our target audience to
not make incorrect assumptions on if they should 'adapt' their usage of the
system in order to make the system understand them. This means that we assume
that the elicited gestures are unbiased.

#### Demographics of participants

|-----|--------|--------------|-----------------------------|---------------------------|
| Age | Gender | Right-handed | Commonly uses digital tools | Used gesture tools before |
|-----|--------|--------------|-----------------------------|---------------------------|
|     |        |              |                             |                           |

## Method

Each participant will be researched individually, isolated from other
participants. The researcher will have multiple symbols prepared, which can be
proposed to the participant, but won't be presented yet. All symbols in this
research are represented by simply showing the gesture with the body - no
animations or videos are used.

Firstly, the participant is asked to think of a possible symbol which fits the
referent, to their opinion. After agreeing upon that they would opt for that
symbol, the researcher presents the other symbols he prepared. The participant
is then asked to choose a symbol from all symbols discussed: those presented and
those the participant came up with. The participant should choose the symbol
which he/she/it believes is the symbol which best fits the referent. The symbol
the participant chooses now, is called the final symbol.

Lastly, the participant is asked to evaluate the final symbol on its complexity.

The results of the interaction with a single participant, include the original
symbol and the final symbol. The symbols should be described with an eye for detail.

## Results

### All symbols

First, we will list all the symbols observed:

### Dissimilarity matrix

To scientifically determine which gestures are to be considered similar, we will
apply a dissimilarity function on every pair of symbols, creating a
dissimilarity matrix.

The dissimilarity function we will use is to manually rate the dissimilarity on
a scale of 0 to 10. While this is no reproducible science, comparing things like
number of joints involved creates results which do not properly represent the
human understanding and associations of the gestures.

TODO HIER DISSIMILARITY MATRIX

### Clustering by dissimilarity

To express which symbols should be considered similar, we must compare all
dissimilarities to a τ, where τ represents the maximal dissimilarity before we
consider two symbols to be different. If the dissimilarity of two symbols is
lower than τ, then we consider them similar. We can express this in a matrix as
well.

TODO HIER EEN MATRIX MET BOOLEANS VOOR SIMILAR OR NOT SIMILAR

This gives us the following clusters:

TODO HIER ALLE SYMBOLS GROEPEREN, ZODAT JE ZE ESSENTIEEL COMBINEERT TOT EEN PAAR
GESTURES DIE WE CONSDIREN

We can treat each cluster as one gesture, so that we can compare these clusters
to determine which cluster is considered to be most fitting for each referent.

### Measure of agreement

To determine which gesture is considered the most fitting for each referent, we
must determine all agreement scores. The agreement score is calculated as
follows:

$$\sum{P_i \in P}{(|P_i|/|P|)^2}$$

Thus, this gives us the following measures of agreement for both referents:

TODO GEEF HIER PER CLUSTER AAN WAT DE MEASURE OF AGREEMNT IS

## Conclusion

TODO CHOOSE GESTURE WITH THE HIGHEST AGREEMENT SCORE.

