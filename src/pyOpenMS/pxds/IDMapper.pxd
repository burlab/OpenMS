from ConsensusMap cimport *
from DefaultParamHandler cimport *
from Feature cimport *
from FeatureMap cimport *
from String cimport *

from ProteinIdentification cimport *
from PeptideIdentification cimport *

from MSExperiment cimport *
from Peak1D cimport *
from ChromatogramPeak cimport *

cdef extern from "<OpenMS/ANALYSIS/ID/IDMapper.h>" namespace "OpenMS":

    cdef cppclass IDMapper(DefaultParamHandler):
        # wrap-inherits:
        #    DefaultParamHandler

        IDMapper() nogil except +
        IDMapper(IDMapper) nogil except +

        void annotate(MSExperiment[Peak1D, ChromatogramPeak] & map_,
                      libcpp_vector[PeptideIdentification] & ids,
                      libcpp_vector[ProteinIdentification] & protein_ids,
                      bool clear_ids,
                      bool mapMS1) nogil except +
					  
        void annotate(MSExperiment[Peak1D, ChromatogramPeak] & map_,
                      FeatureMap & fmap,
                      bool clear_ids,
                      bool mapMS1) nogil except +

        void annotate(FeatureMap & map_,
                      libcpp_vector[PeptideIdentification] & ids,
                      libcpp_vector[ProteinIdentification] & protein_ids,
                      bool use_centroid_rt,
                      bool use_centroid_mz,
                      MSExperiment[Peak1D, ChromatogramPeak] & spectra) nogil except +

        void annotate(ConsensusMap & map_,
                      libcpp_vector[PeptideIdentification] & ids,
                      libcpp_vector[ProteinIdentification] & protein_ids,
                      bool measure_from_subelements,
                      bool annotate_ids_with_subelements, 
                      MSExperiment[Peak1D, ChromatogramPeak] & spectra) nogil except +

cdef extern from "<OpenMS/ANALYSIS/ID/IDMapper.h>" namespace "OpenMS::IDMapper":
    cdef enum Measure:
      MEASURE_PPM = 0,
      MEASURE_DA
