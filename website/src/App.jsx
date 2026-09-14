import { BrowserRouter, Routes, Route } from 'react-router-dom'
import Navbar from './components/Navbar'
import Home from './pages/Home'
import Download from './pages/Download'
import Community from './pages/Community'
import Donate from './pages/Donate'

export default function App() {
  return (
    <BrowserRouter basename="/Maryn_Launcher">
      <Navbar />
      <Routes>
        <Route path="/" element={<Home />} />
        <Route path="/download" element={<Download />} />
        <Route path="/community" element={<Community />} />
        <Route path="/donate" element={<Donate />} />
      </Routes>
    </BrowserRouter>
  )
}