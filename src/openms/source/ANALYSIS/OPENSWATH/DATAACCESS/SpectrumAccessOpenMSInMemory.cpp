// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2015.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Hannes Roest $
// $Authors: Hannes Roest $
// --------------------------------------------------------------------------

#include <OpenMS/ANALYSIS/OPENSWATH/DATAACCESS/SpectrumAccessOpenMSInMemory.h>

namespace OpenMS
{

  SpectrumAccessOpenMSInMemory::SpectrumAccessOpenMSInMemory(OpenSwath::ISpectrumAccess & origin)
  {
    for (Size i = 0; i < origin.getNrSpectra(); ++i)
    {
      spectra_.push_back( origin.getSpectrumById(i) );
      spectra_meta_.push_back( origin.getSpectrumMetaById(i) );
    }
    for (Size i = 0; i < origin.getNrChromatograms(); ++i)
    {
      chromatograms_.push_back( origin.getChromatogramById(i) );
      chromatogram_ids_.push_back( origin.getChromatogramNativeID(i) );
    }
  }

  SpectrumAccessOpenMSInMemory::~SpectrumAccessOpenMSInMemory() {}

  SpectrumAccessOpenMSInMemory::SpectrumAccessOpenMSInMemory(const SpectrumAccessOpenMSInMemory & rhs) :
    spectra_(rhs.spectra_),
    spectra_meta_(rhs.spectra_meta_)
  {
    // TODO this copies everyhing ... 
  }

  boost::shared_ptr<OpenSwath::ISpectrumAccess> SpectrumAccessOpenMSInMemory::lightClone() const
  {
    // TODO this copies everyhing ... 
    return boost::shared_ptr<SpectrumAccessOpenMSInMemory>(new SpectrumAccessOpenMSInMemory(*this));
  }

  OpenSwath::SpectrumPtr SpectrumAccessOpenMSInMemory::getSpectrumById(int id)
  {
    return spectra_[id];
  }

  OpenSwath::SpectrumMeta SpectrumAccessOpenMSInMemory::getSpectrumMetaById(int id) const
  {
    return spectra_meta_[id];
  }

  std::vector<std::size_t> SpectrumAccessOpenMSInMemory::getSpectraByRT(double RT, double deltaRT) const
  {
    OPENMS_PRECONDITION(deltaRT >= 0, "Delta RT needs to be a positive number");

    // we first perform a search for the spectrum that is past the
    // beginning of the RT domain. Then we add this spectrum and try to add
    // further spectra as long as they are below RT + deltaRT.
    std::vector<std::size_t> result;
    OpenSwath::SpectrumMeta s;
    s.RT = RT - deltaRT;
    std::vector< OpenSwath::SpectrumMeta >::const_iterator spectrum = std::upper_bound(
        spectra_meta_.begin(), spectra_meta_.end(), s, typename OpenSwath::SpectrumMeta::RTLess());

    result.push_back(std::distance(spectra_meta_.begin(), spectrum));
    ++spectrum;
    while (spectrum->RT < RT + deltaRT && spectrum != spectra_meta_.end())
    {
      result.push_back(std::distance(spectra_meta_.begin(), spectrum));
      ++spectrum;
    }
    return result;
  }

  size_t SpectrumAccessOpenMSInMemory::getNrSpectra() const
  {
    return spectra_.size();
  }

  OpenSwath::ChromatogramPtr SpectrumAccessOpenMSInMemory::getChromatogramById(int id)
  {
    return chromatograms_[id];
  }

  size_t SpectrumAccessOpenMSInMemory::getNrChromatograms() const
  {
    return chromatograms_.size();
  }

  std::string SpectrumAccessOpenMSInMemory::getChromatogramNativeID(int id) const
  {
    return chromatogram_ids_[id];
  }

} //end namespace OpenMS
